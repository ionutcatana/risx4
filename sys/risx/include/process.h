#ifndef PROCESS_H
#define PROCESS_H 1

#include "commonarch/interrupts.h"
#include "commonarch/paging.h"
#include "risx/config.h"

#include <stddef.h>
#include <stdint.h>

enum procstate {
	UNUSED,
	EMBRYO,
	SLEEPING,
	RUNNABLE,
	RUNNING,
	ZOMBIE
};

struct cpu {
	uint32_t lapicid;
};

struct proc {
	uint32_t pid;
	uint32_t uid;
	uint32_t gid;
	uint64_t entry;
	uint64_t stack;
	enum procstate state;
	struct pagetable* pagetable;
	struct trapframe* trapframe;
	struct proc* parent;
	struct cpu* cpu;
};

struct cpu* cpus(void);
struct proc* proctable(void);
size_t proccount(void);

void initprocesses(void);
void schedule(void);

#endif
