
typedef unsigned long size_t;

// Inline assembly for syscall implementation
static inline long syscall1(long n, long a1) {
    long ret;
    __asm__ __volatile__ (
        "syscall"
        : "=a"(ret)
        : "a"(n), "D"(a1)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline long syscall3(long n, long a1, long a2, long a3) {
    long ret;
    __asm__ __volatile__ (
        "syscall"
        : "=a"(ret)
        : "a"(n), "D"(a1), "S"(a2), "d"(a3)
        : "rcx", "r11", "memory"
    );
    return ret;
}

void _start(void) {
    const char msg[] = "Hello from userspace system call!\n";

    // write(1, msg, sizeof(msg)-1) -> Syscall 1
    syscall3(1, 1, (long)msg, sizeof(msg)-1);

    // exit(0) -> Syscall 60
    syscall1(60, 0);

    // Should not reach here
    for(;;);
}
