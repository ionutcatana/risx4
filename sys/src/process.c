#include <process.h>
#include <risx.h>
#include <arch/x86/registers.h>
#include <commonarch/interrupts.h>
#include <alloc/kpalloc.h> // for allocframe (stack)
#include <alloc/kvalloc.h>
#include <dlmalloc.h>
#include <risx/config.h>
#include <spinlock.h>
#include <elf.h>
#include <arch/x86/trapframe.h>
#include <arch/x86/gdt.h>
#include <alloc/conversions.h>
#include <arch/x86/paging.h>

#include <stdbool.h>
#include <stddef.h>

thread_t    threads[NPROC];
struct cpu  cpus[NCPU];
uint64_t    next_tid = 1;

static spinlock_t sched_lock;

void initprocess(void) {
    initlock(&sched_lock, "sched");
    for (int i = 0; i < NPROC; i++) {
        threads[i].state = UNUSED;
        threads[i].tid = 0;
    }
}

// Simple Round Robin Scheduler
void scheduler(void) {
    struct cpu* c = &cpus[readlapicid()];

    printf("[CPU %lu] Scheduler loop started.\n", readlapicid());

    while (true) {
        // Enable interrupts to allow timer ticks
        enableinterrupts();

        acquire(&sched_lock);

        for (int i = 0; i < NPROC; i++) {
            thread_t* t = &threads[i];

            if (t->state == READY) {
                // Switch to chosen process
                t->state = RUNNING;
                c->thread = t;
                t->cpu_id = readlapicid();

                if (t->cr3 != 0) loadcr3(t->cr3);

                tss_update_rsp0((uintptr_t)t->kstack + 16384);

                swtch(&c->scheduler, t->context);

                loadcr3(readkernelpgtbl());

                // Returns here when thread yields/sleeps
                // Interrupts are DISABLED here (restored from c->scheduler context)
                // Lock is effectively passed back to us

                c->thread = NULL;
            }
        }
        release(&sched_lock);
    }
}

// Switch to scheduler. Must be called with lock held and interrupts disabled.
void sched(void) {
    struct cpu* c = &cpus[readlapicid()];
    if (c->thread) {
        swtch(&c->thread->context, c->scheduler);
    }
}

// Voluntary yield
void yield(void) {
    acquire(&sched_lock);
    struct cpu* c = &cpus[readlapicid()];
    if (c->thread) {
        c->thread->state = READY;
        sched();
    }
    release(&sched_lock);
}

void exit(void) {
    acquire(&sched_lock);
    struct cpu* c = &cpus[readlapicid()];
    if (c->thread) {
        c->thread->state = ZOMBIE;
        printf("[CPU %lu] TID %lu exited.\n", readlapicid(), c->thread->tid);
        sched();
    }
    // Should not return
    panic("Zombie returned!");
}

void sched_release_lock(void) {
    release(&sched_lock);
}

thread_t* kthread_create(void (*start_routine)(void)) {
    thread_t* t = NULL;

    acquire(&sched_lock);
    for (int i = 0; i < NPROC; i++) {
        if (threads[i].state == UNUSED) {
            t = &threads[i];
            t->state = EMBRYO; // Reserve it
            break;
        }
    }
    release(&sched_lock);

    if (!t) return NULL;

    t->tid = __atomic_fetch_add(&next_tid, 1, __ATOMIC_RELAXED);
    t->cpu_id = 0;
    t->cr3 = readkernelpgtbl();
    t->kstack = malloc(16384);
    if (!t->kstack) {
        t->state = UNUSED;
        return NULL;
    }

    uintptr_t sp = (uintptr_t)t->kstack + 16384;

    // Reserve space for trapframe (used by user threads)
    sp -= sizeof(trapframe_t);
    trapframe_t* tf_reserved = (trapframe_t*)sp;

    // Leave space for context
    sp -= sizeof(context_t);
    t->context = (context_t*)sp;

    // Initialize context
    memset(t->context, 0, sizeof(context_t));

    // Set up entry stub
    extern void kthread_entry(void);
    t->context->rip = (uintptr_t)kthread_entry;
    t->context->rbx = (uintptr_t)start_routine;
    // We can also pass tf pointer here if needed, but we calculate it in spawn_process
    t->context->r12 = (uintptr_t)tf_reserved;

    return t;
}

void kthread_start(thread_t* t) {
    acquire(&sched_lock);
    t->state = READY;
    release(&sched_lock);
}

extern void enter_usermode(void);

// Add PAGE_USER if not defined? It is in paging.h included.

void spawn_process(const char* name, void* elf_data) {
    printf("spawn_process: start\n");
    if (!elf_data) panic("spawn_process: invalid elf data");

    thread_t* t = kthread_create(enter_usermode);
    if (!t) panic("spawn_process: failed to create thread");
    printf("spawn_process: thread created %lu\n", t->tid);

    if (elf_load(name, elf_data, t) != 0) {
        printf("spawn_process: failed to load ELF for %s\n", name);
        return;
    }
    printf("spawn_process: elf loaded\n");

    // Allocate User Stack (16KB)
    uintptr_t user_stack_top = 0x700000000000;
    pagetable_t* pml4 = (pagetable_t*)virtual(t->cr3);

    printf("spawn_process: mapping stack. pml4=%p\n", pml4);
    for (int i = 0; i < 4; i++) {
        uintptr_t phys = allocframe(1);
        printf("spawn_process: allocframe returned %lx for stack page %d\n", phys, i);
        if (phys == 0) panic("allocframe failed");
        mappage(pml4, user_stack_top - (i + 1) * PAGE_SIZE, phys, PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER);
        printf("spawn_process: mapped %lx\n", user_stack_top - (i + 1) * PAGE_SIZE);
    }
    printf("spawn_process: stack mapped\n");

    // Fill Trapframe
    trapframe_t* tf = (trapframe_t*)t->context->r12;
    printf("spawn_process: accessing tf at %p\n", tf);
    memset(tf, 0, sizeof(trapframe_t));
    printf("spawn_process: tf memset done\n");

    tf->cs = USER_CODE_SEG | 3;
    tf->ss = USER_DATA_SEG | 3;
    tf->rflags = 0x202; // IF enabled
    tf->rsp = user_stack_top;

    Elf64_Ehdr* ehdr = (Elf64_Ehdr*)elf_data;
    tf->rip = ehdr->e_entry;

    printf("[Process] Spawned %s (TID %lu) Entry: %lx\n", name, t->tid, tf->rip);
    kthread_start(t);
}
