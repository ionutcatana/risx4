#include <syscall.h>
#include <arch/x86/registers.h>
#include <arch/x86/gdt.h>
#include <commonarch/serial.h>
#include <process.h>
#include <risx.h>
#include <console.h>

extern void syscall_entry(void);

void init_syscall_msrs(void) {
    uint64_t efer = rdmsr(MSR_EFER);
    wrmsr(MSR_EFER, efer | EFER_SYSCALL);

    // SYSCALL: CS = STAR[63:48], SS = STAR[63:48] + 8
    // SYSRET:  CS = STAR[47:32] + 16, SS = STAR[47:32] + 8
    // BASE:    0x10 (Data Seg). CS->0x20, SS->0x18.

    uint64_t star = 0;
    star |= ((uint64_t)0x10) << 32;
    star |= ((uint64_t)0x08) << 48;

    wrmsr(MSR_STAR, star);
    wrmsr(MSR_LSTAR, (uint64_t)syscall_entry);
    wrmsr(MSR_SFMASK, (uint64_t)0x200); // Disable IF

    wrmsr(MSR_GS_BASE, (uint64_t)&cpus[readlapicid()]);
    wrmsr(MSR_KERNEL_GS_BASE, 0);
}

void syscall_handler(trapframe_t* tf) {
    printf("Syscall: %lu\n", tf->rax);
    switch (tf->rax) {
    case 1: // write(fd, buf, count)
        {
            int fd = (int)tf->rdi;
            const char* buf = (const char*)tf->rsi;
            size_t count = (size_t)tf->rdx;

            if (fd == 1 || fd == 2) {
                for (size_t i = 0; i < count; i++) {
                    serialputchar(buf[i]);
                }
                tf->rax = count;
            } else {
                tf->rax = -1;
            }
        }
        break;

    case 60: // exit(code)
        exit();
        break;

    default:
        printf("[Syscall] Unknown: %lu\n", tf->rax);
        tf->rax = -1;
        break;
    }
}
