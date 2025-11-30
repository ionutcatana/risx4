#include <arch/x86/acpi.h>
#include <limine.h>
#include <risx.h>

#include <stddef.h>
#include <stdnoreturn.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rsdpreq = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 4
};

rsdp_t* rsdp = NULL;
xsdp_t* xsdp = NULL;
static int version = ACPI_VERSION_1;

void initacpi(void) {
    if (rsdpreq.response == NULL ||
        rsdpreq.response->address == NULL) {
        panic("ACPI is not supported.");
    }

    rsdp = rsdpreq.response->address;
    if (rsdp->revision > 0) {
        version = ACPI_VERSION_SUBSEQUENT;
        xsdp = rsdpreq.response->address;
        rsdp = NULL;
    }
}

int acpiversion(void) {
    return version;
}
