#include <commonarch/interrupts.h>
#include <commonarch/mp.h>
#include <commonarch/serial.h>
#include <console.h>
#include <limine.h>
#include <mm.h>
#include <process.h>
#include <risx.h>
#include <syscall.h>

#if defined(__x86_64__)
#include <arch/x86/acpi.h>
#include <arch/x86/apic.h>
#include <arch/x86/gdt.h>
#include <arch/x86/idt.h>
#include <arch/x86/isr.h>
#include <arch/x86/registers.h>
#endif

#if defined (__aarch64__)
#endif

#if defined (__riscv)
#endif

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(4)

__attribute__((used, section(".limine_requests")))
static volatile struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0
};

noreturn void abort(void);
noreturn void panic(const char* message) {
//  printf("PANIC: %s\n", message);
    (void)message;
    abort();
}

static atomic_bool initialized;

void setup(uintptr_t stacktop) {
    atomic_init(&initialized, false);
    initserial();
    initconsole();
    initprintf();
    printf("stack top: 0x%016lx\n", stacktop);

    initpmm();  printf("physical frame allocator initialized.\n");
    initvmm();  printf("virtual page allocator initialized.\n");
    initheap(); printf("kernel heap initialized.\n");

#if defined (__x86_64__)
    initgdt();  printf("GDT installed.\n");
    initidt();  printf("IDT installed.\n");
//  initisr();  printf("ISR installed.\n");

    initacpi();
//  extern struct rsdp_t* rsdp;
//  extern struct xsdp_t* xsdp;
    switch (acpiversion()) {
    case ACPI_VERSION_1:
//      printf("ACPI 1.0\n");
//      printf("RSDT addr: 0x%016lx\n", rsdp->rsdpaddr);
        break;
    case ACPI_VERSION_SUBSEQUENT:
//      printf("ACPI >= 2.0\n");
//      printf("XSDT addr: 0x%016lx\n", xsdp->xsdtaddr);
        break;
    }

    initapic_discovery();
    initlapic();
    initioapic();

    enableinterrupts();
#endif

#if defined (__aarch64__)
#endif

#if defined (__riscv)
#endif

    initprocess();
    initmp();   printf("multiprocessing initialized.\n");

    printf("setup successful\n");
    atomic_store_explicit(&initialized, true, memory_order_release);
}

noreturn void risx(void) {
    while (!atomic_load_explicit(&initialized, memory_order_acquire));

// initialize syscall msrs on this cpu
#if defined(__x86_64__)
    init_syscall_msrs();
#endif

    printf("[CPU %lu] entered RISX\n", readlapicid());

    // create a dummy kernel thread
    if (readlapicid() == 0) {
        if (module_request.response) {
            struct limine_module_response* modules = module_request.response;
            printf("Modules found: %lu\n", modules->module_count);
            for (uint64_t i = 0; i < modules->module_count; i++) {
                struct limine_file* file = modules->modules[i];
                printf("Loading module: %s\n", file->path);
                spawn_process(file->path, file->address);
                printf("Returned from spawn_process\n");
            }
        }

        // use kernel thread test only if no modules or for testing alongside?
        // for now, let's keep the test if no modules, or disable it.
        // i'll disable it if modules are loaded to reduce noise, or run it anyway.
        // let's comment out the kernel threads for now to focus on user process.
        /*
        extern void kernel_thread_test(void);
        kthread_create(kernel_thread_test);
        kthread_create(kernel_thread_test);
        */
    }

    scheduler();
    panic("unexpected return from scheduler.\n");
}

void kernel_thread_test(void) {
    int id = readlapicid(); // Might change as we migrate
    static atomic_int counter = ATOMIC_VAR_INIT(0);
    int my_count = atomic_fetch_add_explicit(&counter, 1, memory_order_seq_cst);

    printf("Hello from Kernel Thread %d running on CPU %d!\n", my_count, id);

    for (int i = 0; i < 5; i++) {
        yield();
        printf("Thread %d yielded and returned (iter %d)\n", my_count, i);
    }

    printf("Thread %d exiting...\n", my_count);
//  return;
//  will hit the exit handler we pushed
}
