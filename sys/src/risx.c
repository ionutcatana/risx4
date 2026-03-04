#include "acpi.h"
#include "commonarch/abort.h"
#include "commonarch/interrupts.h"
#include "commonarch/mp.h"
#include "commonarch/serial.h"
#include "console.h"
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

noreturn void panic(const char* message) {
    printf("PANIC: %s\n", message);
    abort();
}

static atomic_bool initialized;

void setup(uint64_t stackbase) {
    atomic_init(&initialized, false);
    initprintf();
    initserial();
    initconsole();
    printf("stack top: 0x%016lx\n", stackbase);

    initmodules();

    initpmm();          printf("physical frame allocator initialized.\n");
//  initvmm(stackbase); printf("virtual page allocator initialized.\n");
//  initmp();           printf("multiprocessing initialized.\n");

#if defined (__x86_64__)
    initgdt();  printf("GDT installed.\n");
    initidt();  printf("IDT installed.\n");
//  initacpi();
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
#endif

#if defined (__aarch64__)
#endif

#if defined (__riscv)
#endif

    intenable();

    printf("setup successful.\n");
    atomic_store_explicit(&initialized, true, memory_order_release);
}

noreturn void risx(void) {
    while (!atomic_load_explicit(&initialized, memory_order_acquire));
    printf("[CPU %lu] entered RISX.\n", readlapicid());

    schedule();
    panic("unexpected return from scheduler.\n");
}
