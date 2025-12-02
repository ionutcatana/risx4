#include <alloc/bitmap.h>
#include <limine.h>
#include <risx.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdmreq = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 4
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmapreq = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 4
};

uint32_t*   bitmap = NULL;
size_t      size = 0;
size_t      freepages = 0;
size_t      totalpages = 0;

void setbit(size_t idx) {
    bitmap[idx / 32] |= (1 << (idx % 32));
}

void unsetbit(size_t idx) {
    bitmap[idx / 32] &= ~(1 << (idx % 32));
}

bool checkbit(size_t idx) {
    return bitmap[idx / 32] & (1 << (idx % 32));
}

void initalloc(void) {
    if (hhdmreq.response == NULL) panic("null hddm response");
    if (memmapreq.response == NULL) panic("null memmap response");

    uint64_t offset = hhdmreq.response->offset;
    uintptr_t maxtopaddr = 0;
    for (size_t i = 0; i < memmapreq.response->entry_count; i++)
        if (memmapreq.response->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            uintptr_t topaddr = memmapreq.response->entries[i]->base +
                                memmapreq.response->entries[i]->length;
            if (topaddr > maxtopaddr) maxtopaddr = topaddr;
        }

    size = (maxtopaddr / PAGESIZE / sizeof(uint32_t)) + 1;
    totalpages = (maxtopaddr / PAGESIZE) + 1;
    for (size_t i = 0; i < memmapreq.response->entry_count; i++)
        if (memmapreq.response->entries[i]->type == LIMINE_MEMMAP_USABLE &&
            memmapreq.response->entries[i]->length >= size) {
            bitmap = (uint32_t*)(memmapreq.response->entries[i]->base + offset);
            memset(bitmap, 0xffffffff, size * sizeof(uint32_t));
            break;
        }

    if (bitmap == NULL) panic("bitmap doesn't fit into memory");

    for (size_t i = 0; i < memmapreq.response->entry_count; i++)
        if (memmapreq.response->entries[i]->type == LIMINE_MEMMAP_USABLE)
            for (uint64_t p = 0; p < memmapreq.response->entries[i]->length;
                p += PAGESIZE) {
                uintptr_t physaddr = memmapreq.response->entries[i]->base + p;
                uintptr_t startaddr = (uintptr_t)bitmap - offset;
                uintptr_t finaladdr = startaddr + size * sizeof(uint32_t);
                if (physaddr < startaddr || physaddr >= finaladdr) {
                    unsetbit(physaddr / PAGESIZE);
                    freepages++;
                }
            }
}

uintptr_t allocframe(size_t count) {
    if (count == 0) panic("allocated 0 page frames.");
    if (count > totalpages) panic("requested more memory than installed.");

    for (size_t i = 0; i + count <= totalpages; i++) {
        bool found = true;
        for (size_t k = 0; k < count; k++) if (checkbit(i + k) != 0) {
            found = false;
            i += k;
            break;
        }

        if (found) {
            for (size_t k = 0; k < count; k++) setbit(i + k);
            freepages -= count;
            return (i * PAGESIZE);
        }
    }

    panic("out of contiguous memory.");
}

void freeframe(uintptr_t frameptr, size_t count) {
    uint64_t frameidx = frameptr / 4096;
    for (uint64_t i = frameidx; i < frameidx + count; i++) unsetbit(i);
    freepages += count;
}
