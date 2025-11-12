#include <devices/serial.h>
#include <devices/console.h>
#include <libk/kstdlib.h>
#include <limine.h>

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

// __attribute__((used, section(".limine_requests")))
// static volatile struct limine_bootloader_info_request info_request = {
//     .id = LIMINE_BOOTLOADER_INFO_REQUEST,
//     .revision = 0,
// };

// __attribute__((used, section(".limine_requests")))
// static volatile struct limine_executable_cmdline_request cmdline_request = {
//     .id = LIMINE_EXECUTABLE_CMDLINE_REQUEST,
//     .revision = 0
// };

// // __attribute__((used, section(".limine_requests")))
// // static volatile struct limine_memory_map_request memory_map_request = {
// //     .id = LIMINE_MEMORY_MAP_REQUEST,
// //     .revision = 0
// // };

// // __attribute__((used, section(".limine_requests")))
// // static volatile struct limine_mp_request mp_request = {
// //     .id = LIMINE_MP_REQUEST,
// //     .revision = 0,
// //     .flags = 0
// // };

// __attribute__((used, section(".limine_requests")))
// static volatile struct limine_memmap_request memmap_request = {
//     .id = LIMINE_MEMMAP_REQUEST,
//     .revision = 0
// };

// __attribute__((used, section(".limine_requests")))
// static volatile struct limine_rsdp_request rsdp_request = {
//     .id = LIMINE_RSDP_REQUEST,
//     .revision = 0
// };

// // __attribute__((used, section(".limine_requests")))
// // static volatile struct limine_date_at_boot_request date_at_boot_request = {
// //     .id = LIMINE_DATE_AT_BOOT_REQUEST,
// //     .revision = 0
// // };

noreturn void setup(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        abort();
    }

    initserial();
    initconsole();

    // get the ball rolling
    risx();
}
