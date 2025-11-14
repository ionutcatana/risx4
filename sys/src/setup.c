#include <devices/console.h>
#include <devices/serial.h>
#include <libk/kstdio.h>
#include <libk/kstdlib.h>
#include <limine.h>
#include <mm.h>
#include <stdint.h>

#ifdef __x86_64__
#include <x86/gdt.h>
#include <x86/idt.h>
#include <x86/isr.h>
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdnoreturn.h>

// main.c
noreturn void risx(void);

// markers
__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

// requests
__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(0);

noreturn void setup(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        abort();
    }

    initserial();
    initconsole();

    initmm();
    extern struct limine_memmap_entry memmap_entries[];
    extern uint64_t memmap_entry_count;

    kprintf("Usable memory map entries: %d\n", memmap_entry_count);
    for (size_t i = 0; i < memmap_entry_count; i++) {
        kprintf("Base: 0x%x, Length: 0x%x\n",
                memmap_entries[i].base,
                memmap_entries[i].length);
    }

#ifdef __x86_64__
    initgdt();
    initidt();
    // initisr();
#endif

    // get the ball rolling
    risx();
}
