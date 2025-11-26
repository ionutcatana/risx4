#include <risx.h>

#include <interrupts.h>
#include <mm.h>

#include <limine.h>

// I/O
#include <console.h>
#include <serial.h>

#if defined(__x86_64__)
#include <arch/x86/acpi.h>
#include <arch/x86/gdt.h>
#include <arch/x86/idt.h>
#include <arch/x86/isr.h>
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(4);

void setup(void) {
    initserial();
    initconsole();

#if defined (__x86_64__)
    initgdt();
    initidt();
//  initisr();

    initacpi();
    extern struct rsdp_t* rsdp;
    extern struct xsdp_t* xsdp;
    switch (acpiversion()) {
    case ACPI_VERSION_1:
        printf("ACPI 1.0\n");
        printf("RSDT addr: 0x%016lx\n", rsdp->rsdpaddr);
        break;
    case ACPI_VERSION_SUBSEQUENT:
        printf("ACPI >= 2.0\n");
        printf("XSDT addr: 0x%016lx\n", xsdp->xsdtaddr);
        break;
    }

    enableinterrupts();
#endif

    initmm();
    printf("Setup successful\n");
}

noreturn void risx(uintptr_t stacktop) {
    setup();
#if defined (RISXDEBUG)
    printf("Entered RISX (debug profile)\n");
#else
    printf("Entered RISX (release profile)\n");
#endif
    // stop warning
    printf("stack top: 0x%016lx\n", stacktop);

    // test a page fault
    uintptr_t ptr = 0xdeadbeef;
    *((uint64_t*)ptr) = 42;

    while(true);

    panic("Unexpected return from scheduler.\n");
}


