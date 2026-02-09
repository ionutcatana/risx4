#ifndef PROCESS_H
#define PROCESS_H 1

#include <arch/x86/context.h>
#include <risx.h>
#include <stdbool.h>

typedef enum {
    UNUSED,
    EMBRYO,
    READY,
    RUNNING,
    SLEEPING,
    ZOMBIE
} thread_state_t;

typedef struct thread {
    uint64_t        tid;
    thread_state_t  state;
    void*           kstack;
    context_t*      context;
    uintptr_t       cr3;
    uint64_t        cpu_id;
} thread_t;

// Context switch
// swtch(struct context **old, struct context *new);
void swtch(context_t** old, context_t* new);

// Process management
void    initprocess(void);
void    scheduler(void);
void    sched(void);    // Internal scheduler switch (interrupts disabled)
void    yield(void);    // Voluntary yield
void    exit(void);
void    sched_release_lock(void); // Used by new threads
void    spawn_process(const char* name, void* elf_data);

thread_t* kthread_create(void (*start_routine)(void));
void kthread_start(thread_t* t);

// Current cpu structure (simplification)
struct cpu {
    uint64_t    scratch_rsp; // Offset 0
    thread_t*   thread;      // Offset 8
    context_t*  scheduler;
};

extern struct cpu cpus[];

#endif
