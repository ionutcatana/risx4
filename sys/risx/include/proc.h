#ifndef PROC_H
#define PROC_H 1

// TODO: wip
enum procstate {
    UNUSED,
    EMBRYO,
    SLEEPING,
    RUNNABLE,
    RUNNING,
    ZOMBIE
};

struct proc {
    uint32_t pid;
    uint32_t uid;
    uint32_t gid;
    uint64_t stack;
    enum procstate state;
    struct pagetable* pagetable;
    struct trapframe* trapframe;
    struct proc* parent;
    struct cpu* cpu;
};

struct cpu {
    uint32_t lapicid;
};

#endif
