#include "proc.h"
#include "risx.h"

struct cpu cpus[NCPU];
struct proc proc[NTASK + NPROC];
