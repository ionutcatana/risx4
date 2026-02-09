#include <elf.h>
#include <process.h>
#include <alloc/kvalloc.h>
#include <alloc/kpalloc.h>
#include <alloc/conversions.h>
#include <arch/x86/paging.h>
#include <stddef.h>
#include <risx.h>
#include <commonarch/paging.h>

#define ALIGN_DOWN(x, a) ((x) & ~((a) - 1))
#define ALIGN_UP(x, a) (((x) + (a) - 1) & ~((a) - 1))

int elf_load(const char* name, void* buffer, thread_t* thread) {
    Elf64_Ehdr* ehdr = (Elf64_Ehdr*)buffer;
    printf("elf_load: verifying magic\n");

    // Verify ELF Magic
    if (memcmp(ehdr->e_ident, "\x7f\x45\x4c\x46", 4) != 0) {
        printf("elf_load: Invalid ELF magic for %s\n", name);
        return -1;
    }

    // Create new address space
    printf("elf_load: creating new pml4\n");
    thread->cr3 = vmm_new_pml4();
    printf("elf_load: pml4 created at %lx\n", thread->cr3);
    pagetable_t* pml4 = (pagetable_t*)virtual(thread->cr3);

    Elf64_Phdr* phdr = (Elf64_Phdr*)((uintptr_t)buffer + ehdr->e_phoff);

    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            uintptr_t vaddr = phdr[i].p_vaddr;
            uintptr_t aligned_vaddr = ALIGN_DOWN(vaddr, PAGE_SIZE);

            size_t memsz = phdr[i].p_memsz;
            size_t filesz = phdr[i].p_filesz;
            size_t file_offset = phdr[i].p_offset;

            // Calculate end
            uintptr_t end_vaddr = vaddr + memsz;
            uintptr_t aligned_end = ALIGN_UP(end_vaddr, PAGE_SIZE);

            size_t total_size = aligned_end - aligned_vaddr;

            uint64_t flags = PAGE_PRESENT | PAGE_USER;
            if (phdr[i].p_flags & PF_W) flags |= PAGE_WRITABLE;
            if (phdr[i].p_flags & PF_X) {
                // If executable, don't set NX
            } else {
                flags |= PAGE_NO_EXECUTE;
            }

            // Map pages
            for (size_t j = 0; j < total_size; j += PAGE_SIZE) {
                uintptr_t curr_vaddr = aligned_vaddr + j;
                uintptr_t phys = allocframe(1);
                void* virt = virtual(phys);
                memset(virt, 0, PAGE_SIZE);

                uintptr_t copy_start_vaddr = curr_vaddr < vaddr ? vaddr : curr_vaddr;
                uintptr_t copy_end_vaddr = (curr_vaddr + PAGE_SIZE) > (vaddr + filesz) ? (vaddr + filesz) : (curr_vaddr + PAGE_SIZE);

                if (copy_start_vaddr < copy_end_vaddr) {
                    size_t copy_len = copy_end_vaddr - copy_start_vaddr;
                    size_t page_off = copy_start_vaddr - curr_vaddr;
                    size_t file_off = file_offset + (copy_start_vaddr - vaddr);

                    memcpy((void*)((uintptr_t)virt + page_off), (void*)((uintptr_t)buffer + file_off), copy_len);
                }

                mappage(pml4, curr_vaddr, phys, flags);
            }
        }
    }

    return 0;
}
