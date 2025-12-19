#include <commonarch/interrupts.h>
#include <commonarch/mp.h>
#include <commonarch/serial.h>
#include <console.h>
#include <mm.h>
#include <risx.h>

#include <limine.h>

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

    enableinterrupts();
#endif

    initpmm();  printf("physical frame allocator initialized.\n");
    initvmm();  printf("virtual page allocator initialized.\n");
    printf("setup successful\n");
}

noreturn void risx(uintptr_t stacktop) {
    setup();
#if defined (RISXDEBUG)
    printf("entered RISX (debug profile)\n");
    printf("stack top: 0x%016lx\n", stacktop);
#else
    printf("entered RISX (release profile)\n");
#endif
    enumeratecpus();

//  printf("\033[31mThis text is red!\033[0m\n");
//  printf("This text is not red\n");
    while(true);
    panic("unexpected return from scheduler.\n");
}


