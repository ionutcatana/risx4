#include "acpi.h"
#include "limine.h"
#include "panic.h"
#include "risx.h"
#include <stddef.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rsdpreq = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = LIMINE_API_REVISION
};

struct rsdp* rsdp = NULL;
struct xsdp* xsdp = NULL;
static int version = ACPI_VERSION_1;

void initacpi(void)
{
    if (rsdpreq.response == NULL || rsdpreq.response->address == NULL)
        panic("ACPI is not supported.");

    rsdp = rsdpreq.response->address;
    if (rsdp->revision > 0) {
        version = ACPI_VERSION_SUBSEQUENT;
        xsdp = rsdpreq.response->address;
        rsdp = NULL;
    }
}

int acpiversion(void)
{
    return version;
}
