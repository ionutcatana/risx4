#include "acpi.h"
#include "commonarch/abort.h"
#include "commonarch/interrupts.h"
#include "commonarch/mp.h"
#include "commonarch/serial.h"
#include "commonarch/console.h"
#include "lib/printf.h"
#include "limine.h"
#include "mm.h"
#include "modules.h"
#include "panic.h"
#include "process.h"
#include "risx.h"

#if defined(__x86_64__)
#include "arch/x86_64/specific/gdt.h"
#include "arch/x86_64/specific/idt.h"
#include "arch/x86_64/specific/registers.h"
#endif

#if defined (__aarch64__)
#endif

#if defined (__riscv)
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(4)

noreturn void panic(const char* message)
{
    printf("PANIC: %s\n", message);
    abort();
}

void boostrap(void)
{
    /* basic display and communication                                        */
    initprintf();
    initserial();
    initconsole();

    /* servers and drivers                                                    */
    initmodules();

    /* memory management mechanisms                                           */
    initpmm(); printf("[CPU %llu] physical frame allocator initialized.\n", readlapicid());
    initvmm(); printf("[CPU %llu] virtual page allocator initialized.\n", readlapicid());

    /* acpi table parsing                                                     */
    initacpi();

    /* start all cores                                                        */
    initmp();
}

void setup(struct limine_mp_info* info)
{
#if defined (__x86_64__)
    initgdt(); printf("[CPU %llu] GDT installed.\n", info->processor_id);
    initidt(); printf("[CPU %llu] IDT installed.\n", info->processor_id);
#endif

    intenable();

    printf("[CPU %llu] setup successful.\n", info->processor_id);
}

noreturn void risx(void)
{
    /* start the ball rolling.                                                */
    schedule();
    panic("unexpected return from scheduler.\n");
}
