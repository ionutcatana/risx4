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

struct xsdp* xsdp = NULL;

void initacpi(void)
{
    if (rsdpreq.response == NULL || rsdpreq.revision < ACPI_VERSION_SUBSEQUENT)
        panic("ACPI 2.0 is not supported.");

    xsdp = rsdpreq.response->address;
}

