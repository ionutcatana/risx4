#include "alloc/conversions.h"
#include "alloc/kpalloc.h"
#include "alloc/kvalloc.h"
#include "commonarch/paging.h"
#include "elf.h"
#include "elfloader.h"
#include "lib/printf.h"
#include "lib/string.h"
#include "memlayout.h"
#include "modules.h"
#include "panic.h"

#if defined(__x86_64__)
#include "arch/x86_64/paging.h"
#endif

#include <stddef.h>
#include <stdint.h>

static struct pagetable*
mkusertable(void)
{
    struct pagetable *kt = kerneltable();
    uint64_t phys = allocframe(1);
    struct pagetable *ut = virtual(phys);

    memset(ut, 0, sizeof(struct pagetable));

    // share the higher half (indices 256..511)
    for (int i = 256; i < NENTRIES_PT; i++)
        ut->entries[i] = kt->entries[i];

    return ut;
}

static uint64_t
elfflags(uint32_t pflags)
{
    uint64_t f = PAGE_PRESENT | PAGE_USER;

    if (pflags & ELF_PROGRAM_FLAG_WRITE)
        f |= PAGE_WRITABLE;
    if (!(pflags & ELF_PROGRAM_FLAG_EXECUTE))
        f |= PAGE_NO_EXECUTE;

    return f;
}

static bool
validelf(const struct elfheader* eh)
{
    const uint8_t magic[] = ELF_IDENTIFIER_MAGIC;

    return (eh != NULL)
        && (memcmp(eh->ei_magic, magic, 4) == 0)
        && (eh->ei_class == ELF_IDENTIFIER_CLASS_64_BIT)
        && (eh->ei_data == ELF_IDENTIFIER_DATA_LE)
        && (eh->ei_version == ELF_IDENTIFIER_VERSION)
        && (eh->e_type == ELF_IDENTIFIER_TYPE_EXEC)
#if defined(__x86_64__)
        && (eh->e_machine == ELF_IDENTIFIER_MACHINE_AMD64)
#elif defined(__aarch64__)
        && (eh->e_machine == ELF_IDENTIFIER_MACHINE_ARM64)
#elif defined(__riscv)
        && (eh->e_machine == ELF_IDENTIFIER_MACHINE_RISCV)
#endif
        ;
}

static void
loadsegment(struct pagetable *ut, const struct elfprogramheader *ph, const void *filebase)
{
    if (ph->p_memsz == 0)
        return;

    uint64_t vbase = ph->p_vaddr & ~((uint64_t)PAGE_SIZE - 1);
    uint64_t vend  = (ph->p_vaddr + ph->p_memsz + PAGE_SIZE - 1) &
             ~((uint64_t)PAGE_SIZE - 1);
    size_t npages = (vend - vbase) / PAGE_SIZE;

    uint64_t pflags = elfflags(ph->p_flags);

    // allocate contiguous frames for the whole segment
    uint64_t phys = allocframe(npages);
    if (phys == 0)
        panic("ELF: failed to allocate segment frames");

    void *dst = virtual(phys);
    memset(dst, 0, npages * PAGE_SIZE);

    // copy file data into the allocated region at the correct offset
    uint64_t off_in_page = ph->p_vaddr - vbase;
    const uint8_t *src = (const uint8_t *)filebase + ph->p_offset;
    memcpy((uint8_t *)dst + off_in_page, src, ph->p_filesz);

    // map each page into the user table
    for (size_t i = 0; i < npages; i++) {
        uint64_t va = vbase + i * PAGE_SIZE;
        uint64_t pa = phys + i * PAGE_SIZE;
        mappage(ut, va, pa, pflags);
    }
}

void
loadelf(const struct module *mod, struct elfimage *out)
{
    if (mod == NULL || out == NULL || mod->size < sizeof(struct elfheader))
        panic("ELF: invalid arguments to loadelf");

    const struct elfheader *eh = (const struct elfheader *)mod->virtaddr;

    if (!validelf(eh))
        panic("ELF: validation failed for module");

    // sanity check: program header table must fit in module
    uint64_t phtabend = eh->e_phoff + (uint64_t)eh->e_phnum * eh->e_phentsize;
    if (phtabend > mod->size)
        panic("ELF: phdr table overflows module");

    size_t user_stack_pages = STACK_SIZE / PAGE_SIZE;

    struct pagetable *ut = mkusertable();

    const struct elfprogramheader *phdrs = (const struct elfprogramheader *)((const uint8_t *)mod->virtaddr + eh->e_phoff);

    for (uint16_t i = 0; i < eh->e_phnum; i++) {
        if (phdrs[i].p_type != ELF_PROGRAM_TYPE_LOAD)
            continue;

        // bounds-check the segment against the module size
        if (phdrs[i].p_offset + phdrs[i].p_filesz > mod->size)
            panic("ELF: segment overflows module");

        loadsegment(ut, &phdrs[i], mod->virtaddr);
    }

    // allocate a user-mode stack. place a guard page below it.
    // the stack sits just below the top of the lower-half canonical address range.
    uint64_t stkphys = allocframe(user_stack_pages);
    if (stkphys == 0)
        panic("ELF: failed to allocate user stack");

    memset(virtual(stkphys), 0, user_stack_pages * PAGE_SIZE);

    uint64_t stktop = STACKUBASE0;
    uint64_t stkbot = stktop - STACK_SIZE;

    for (size_t i = 0; i < user_stack_pages; i++)
        mappage(ut, stkbot + i * PAGE_SIZE, stkphys + i * PAGE_SIZE, PAGE_USER_STACK);

    // guard page: simply not mapped, so any access faults
    out->entry     = eh->e_entry;
    out->pagetable = ut;
    out->stack     = stktop;

    printf("ELF: loaded %s (entry %#016llx, stack %#016llx)\n",
        mod->name,
        out->entry,
        out->stack);
}
