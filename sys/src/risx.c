#include <libk/kstdio.h>
#include <libk/kstdlib.h>
#include <mm.h>
#include <risx.h>
#include <serial.h>

#ifdef __x86_64__
#include <arch/x86/acpi.h>
#include <arch/x86/gdt.h>
#include <arch/x86/idt.h>
#include <arch/x86/interrupts.h>
#include <arch/x86/isr.h>
#endif

#include <limine.h>
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

#if defined (__x86_64__)
    initgdt();
    initidt();
    // initisr();

    initacpi();
    // extern struct rsdp* rsdp;
    // extern struct xsdp* xsdp;
    // switch (acpiversion()) {
    // case ACPI_VERSION_1:
    //     kprintf("ACPI 1.0\n");
    //     kprintf("RSDT addr: 0x%x\n", rsdp->rsdpaddr);
    //     break;
    // case ACPI_VERSION_SUBSEQUENT:
    //     kprintf("ACPI >= 2.0\n");
    //     kprintf("XSDT addr: 0x%x\n", xsdp->xsdtaddr);
    //     break;
    // }

    enableinterrupts();
#endif

    initmm();
    extern struct limine_memmap_entry memmap_entries[];
    extern uint64_t memmap_entry_count;

    kprintf("Usable memory map entries: %d\n", memmap_entry_count);
    for (size_t i = 0; i < memmap_entry_count; i++) {
        switch (memmap_entries[i].type) {
        case LIMINE_MEMMAP_USABLE:
            kprintf("Area `usable`: ");
            kprintf("Base: 0x%x, Length: 0x%x\n",
                memmap_entries[i].base,
                memmap_entries[i].length);
            break;
        case LIMINE_MEMMAP_RESERVED:
            kprintf("Area `reserved`: ");
            kprintf("Base: 0x%x, Length: 0x%x\n",
                memmap_entries[i].base,
                memmap_entries[i].length);
            break;
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            kprintf("Area `acpi reclaimable`: ");
            kprintf("Base: 0x%x, Length: 0x%x\n",
                memmap_entries[i].base,
                memmap_entries[i].length);
            break;
        case LIMINE_MEMMAP_ACPI_NVS:
            kprintf("Area `acpi nvs`: ");
            kprintf("Base: 0x%x, Length: 0x%x\n",
                memmap_entries[i].base,
                memmap_entries[i].length);
            break;
        case LIMINE_MEMMAP_BAD_MEMORY:
            kprintf("Area `bad memory`: ");
            kprintf("Base: 0x%x, Length: 0x%x\n",
                memmap_entries[i].base,
                memmap_entries[i].length);
            break;
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            kprintf("Area `bootloader reclaimable`: ");
            kprintf("Base: 0x%x, Length: 0x%x\n",
                memmap_entries[i].base,
                memmap_entries[i].length);
            break;
        case LIMINE_MEMMAP_KERNEL_AND_MODULES:
            kprintf("Area `executable and modules`: ");
            kprintf("Base: 0x%x, Length: 0x%x\n",
                memmap_entries[i].base,
                memmap_entries[i].length);
            break;
        case LIMINE_MEMMAP_FRAMEBUFFER:
            kprintf("Area `framebuffer`: ");
            kprintf("Base: 0x%x, Length: 0x%x\n",
                memmap_entries[i].base,
                memmap_entries[i].length);
            break;
        case LIMINE_MEMMAP_ACPI_TABLES:
            kprintf("Area `acpi tables`: ");
            kprintf("Base: 0x%x, Length: 0x%x\n",
                memmap_entries[i].base,
                memmap_entries[i].length);
            break;
        }
    }

    kprintf("Setup successful\n");
}

noreturn void risx(void) {
    setup();
    kprintf("Entered RISX\n");

    while(true);

    panic("Unexpected return from scheduler");
}


