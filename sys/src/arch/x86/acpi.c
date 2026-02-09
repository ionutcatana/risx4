#include <arch/x86/acpi.h>
#include <alloc/conversions.h>
#include <limine.h>
#include <risx.h>

#include <stddef.h>
#include <stdnoreturn.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rsdpreq = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 4
};

rsdp_t*     rsdp    = NULL;
xsdp_t*     xsdp    = NULL;
static int  version = ACPI_VERSION_1;

void initacpi(void) {
    if (rsdpreq.response == NULL || rsdpreq.response->address == NULL)
        panic("ACPI is not supported.");

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

void* acpifindtable(const char* signature) {
    size_t entries = 0;
    sdpheader_t* header = NULL;

    if (version == ACPI_VERSION_1 && rsdp) {
        rsdt* rsdtval = (rsdt*)virtual(rsdp->rsdpaddr);
        entries = (rsdtval->header.length - sizeof(rsdtval->header)) / 4;

        for (size_t i = 0; i < entries; i++) {
             header = (sdpheader_t*)virtual(rsdtval->sdtaddr[i]);
             if (memcmp(header->signature, signature, 4) == 0) return (void*)header;
        }
    } else if (xsdp) {
        xsdt* xsdtval = (xsdt*)virtual(xsdp->xsdtaddr);
        entries = (xsdtval->header.length - sizeof(xsdtval->header)) / 8;

        for (size_t i = 0; i < entries; i++) {
             header = (sdpheader_t*)virtual(xsdtval->sdtaddr[i]);
             if (memcmp(header->signature, signature, 4) == 0) return (void*)header;
        }
    }
    return NULL;
}
