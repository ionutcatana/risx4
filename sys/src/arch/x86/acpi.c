#include <arch/x86/acpi.h>
#include <libk/kstdlib.h>
#include <risx.h>

#include <limine.h>
#include <stddef.h>
#include <stdnoreturn.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request limine_rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 4
};

struct rsdp* rsdp = NULL;
struct xsdp* xsdp = NULL;
static int version = ACPI_VERSION_1;

void initacpi(void) {
    if (limine_rsdp_request.response == NULL ||
        limine_rsdp_request.response->address == NULL) {
        panic("ACPI is not supported");
    }

    rsdp = limine_rsdp_request.response->address;
    if (rsdp->revision > 0) {
        version = ACPI_VERSION_SUBSEQUENT;
        xsdp = limine_rsdp_request.response->address;
        rsdp = NULL;
    }
}

int acpiversion(void) {
    return version;
}
