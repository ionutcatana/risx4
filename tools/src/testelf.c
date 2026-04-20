#include "elf.h"
#include <stdio.h>
#include <malloc.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <elf_file>\n", argv[0]);
        return 1;
    }

    FILE* f = fopen(argv[1], "rb");
    struct elfheader* header = malloc(sizeof(struct elfheader));
    fread(header, sizeof(struct elfheader), 1, f);

    printf("ELF Header:\n");
    printf("* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\n");
    printf("Magic: %02x,%c,%c,%c\n", header->ei_magic[0],
                                     header->ei_magic[1],
                                     header->ei_magic[2],
                                     header->ei_magic[3]);
    printf("Class: ");
    switch (header->ei_class) {
    case ELF_IDENTIFIER_CLASS_32_BIT:
        printf("32-bit\n");
        break;
    case ELF_IDENTIFIER_CLASS_64_BIT:
        printf("64-bit\n");
        break;
    }

    printf("Data: ");
    switch (header->ei_data) {
    case ELF_IDENTIFIER_DATA_LE:
        printf("little endian\n");
        break;
    case ELF_IDENTIFIER_DATA_BE:
        printf("big endian\n");
        break;
    }

    printf("Version: %u\n", header->ei_version);

    printf("OS ABI: ");
    switch (header->ei_osabi) {
    case ELF_IDENTIFIER_OSABI_SYSV:
        printf("System V\n");
        break;
    case ELF_IDENTIFIER_OSABI_NETBSD:
        printf("NetBSD\n");
        break;
    case ELF_IDENTIFIER_OSABI_LINUX:
        printf("Linux\n");
        break;
    case ELF_IDENTIFIER_OSABI_GNUHURD:
        printf("GNU Hurd\n");
        break;
    case ELF_IDENTIFIER_OSABI_SOLARIS:
        printf("Solaris\n");
        break;
    case ELF_IDENTIFIER_OSABI_AIX:
        printf("AIX\n");
        break;
    case ELF_IDENTIFIER_OSABI_FREEBSD:
        printf("FreeBSD\n");
        break;
    case ELF_IDENTIFIER_OSABI_OPENBSD:
        printf("OpenBSD\n");
        break;
    default:
        printf("Unknown\n");
        break;
    }

    printf("ABI version: %u\n", header->ei_abiversion);

    printf("Type: ");
    switch(header->e_type){
    case ELF_IDENTIFIER_TYPE_NONE:
        printf("Unknown\n");
        break;
    case ELF_IDENTIFIER_TYPE_REL:
        printf("Relocatable file\n");
        break;
    case ELF_IDENTIFIER_TYPE_EXEC:
        printf("Executable file\n");
        break;
    case ELF_IDENTIFIER_TYPE_DYN:
        printf("Shared object\n");
        break;
    case ELF_IDENTIFIER_TYPE_CORE:
        printf("Core file\n");
        break;
    case ELF_IDENTIFIER_TYPE_LOOS:
        printf("Range lower bound (OS specific)\n");
        break;
    case ELF_IDENTIFIER_TYPE_HIOS:
        printf("Range upper bound (OS specific)\n");
        break;
    case ELF_IDENTIFIER_TYPE_LOPROC:
        printf("Range lower bound (processor specific)\n");
        break;
    case ELF_IDENTIFIER_TYPE_HIPROC:
        printf("Range upper bound (processor specific)\n");
        break;
    }

    printf("Machine: ");
    switch(header->e_machine) {
    case ELF_IDENTIFIER_MACHINE_NONE:
        printf("No specific instruction set\n");
        break;
    case ELF_IDENTIFIER_MACHINE_AMD64:
        printf("AMD64\n");
        break;
    case ELF_IDENTIFIER_MACHINE_ARM64:
        printf("ARM64\n");
        break;
    case ELF_IDENTIFIER_MACHINE_RISCV:
        printf("RISC-V\n");
        break;
    default:
        printf("Other\n");
        break;
    }

    printf("Version: %u\n", header->e_version);
    printf("Entry point address %016llx\n", header->e_entry);
    printf("Program header offset: %016llx\n", header->e_phoff);
    printf("Section header offset: %016llx\n", header->e_shoff);

    printf("Flags: %u (hex: %08lx)\n", header->e_flags,
                                       header->e_flags);

    printf("Header size: %u (hex: %2lx)\n", header->e_ehsize,
                                            header->e_ehsize);

    printf("Program header size: %u (hex: %2lx)\n", header->e_phentsize,
                                                    header->e_phentsize);

    printf("Program header entries: %u\n", header->e_phnum);

    printf("Section header size: %u (hex: %2lx)\n", header->e_shentsize,
                                                    header->e_shentsize);

    printf("Section header entries: %u\n", header->e_shnum);
    printf("Section names index: %u\n", header->e_shstrndx);

    fseek(f, header->e_phoff, SEEK_SET);
    struct elfprogramheader* programheaders = malloc(sizeof(struct elfprogramheader) * header->e_phnum);

    /* read all program headers at once */
    fread(programheaders, sizeof(struct elfprogramheader), header->e_phnum, f);

    for (size_t i = 0; i < header->e_phnum; i++) {
        printf("\n");
        printf("Program header %lu:\n", i);
        printf("* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\n");

        printf("Type: ");
        switch (programheaders[i].p_type) {
        case ELF_PROGRAM_TYPE_NULL:
            printf("Unused.\n");
            break;
        case ELF_PROGRAM_TYPE_LOAD:
            printf("Loadable segment\n");
            break;
        case ELF_PROGRAM_TYPE_DYNAMIC:
            printf("Dynamic linking information\n");
            break;
        case ELF_PROGRAM_TYPE_INTERP:
            printf("Interpreter information\n");
            break;
        case ELF_PROGRAM_TYPE_NOTE:
            printf("Auxiliary information\n");
            break;
        case ELF_PROGRAM_TYPE_SHLIB:
            printf("Reserved\n");
            break;
        case ELF_PROGRAM_TYPE_PHDR:
            printf("Program header table\n");
            break;
        case ELF_PROGRAM_TYPE_TLS:
            printf("Thread-local storage template\n");
            break;
        case ELF_PROGRAM_TYPE_LOOS:
            printf("Range lower bound (OS specific)\n");
            break;
        case ELF_PROGRAM_TYPE_HIOS:
            printf("Range upper bound (OS specific)\n");
            break;
        case ELF_PROGRAM_TYPE_LOPROC:
            printf("Range lower bound (processor specific)\n");
            break;
        case ELF_PROGRAM_TYPE_HIPROC:
            printf("Range upper bound (processor specific)\n");
            break;
        }

        printf("Flags: %u (hex: %08lx)\n", programheaders[i].p_flags,
                                           programheaders[i].p_flags);
        printf("Offset: %016llx\n", programheaders[i].p_offset);
        printf("Virtual address: %016llx\n", programheaders[i].p_vaddr);
        printf("Physical address: %016llx\n", programheaders[i].p_paddr);
        printf("File size: %016llx\n", programheaders[i].p_filesz);
        printf("Memory size: %016llx\n", programheaders[i].p_memsz);
        printf("Alignment: %016llx\n", programheaders[i].p_align);
    }

    fseek(f, header->e_shoff, SEEK_SET);
    struct elfsectionheader* sectionheaders = malloc(sizeof(struct elfsectionheader) * header->e_shnum);

    /* read all section headers at once so we can locate the section-name string table */
    fread(sectionheaders, sizeof(struct elfsectionheader), header->e_shnum, f);

    /* load section header string table (shstrtab) */
    char* shstrtab = NULL;
    uint64_t shstr_size = 0;
    if (header->e_shstrndx < header->e_shnum) {
        struct elfsectionheader* shstr_sh = &sectionheaders[header->e_shstrndx];
        shstr_size = shstr_sh->sh_size;
        shstrtab = malloc(shstr_size + 1);
        if (shstrtab) {
            fseek(f, shstr_sh->sh_offset, SEEK_SET);
            fread(shstrtab, 1, shstr_size, f);
            shstrtab[shstr_size] = '\0';
        }
    }

    for (size_t i = 0; i < header->e_shnum; i++) {
        printf("\n");
        printf("Section header %lu:\n", i);
        printf("* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\n");
        if (shstrtab && sectionheaders[i].sh_name < shstr_size) {
            printf("Name: %s\n", shstrtab + sectionheaders[i].sh_name);
        } else {
            printf("Name offset: %u\n", sectionheaders[i].sh_name);
        }

        printf("Type: ");
        switch (sectionheaders[i].sh_type) {
        case ELF_SECTION_TYPE_NULL:
            printf("Unused\n");
            break;
        case ELF_SECTION_TYPE_PROGBITS:
            printf("Program data\n");
            break;
        case ELF_SECTION_TYPE_SYMTAB:
            printf("Symbol table\n");
            break;
        case ELF_SECTION_TYPE_STRTAB:
            printf("String table\n");
            break;
        case ELF_SECTION_TYPE_RELA:
            printf("Relocation entries with addends\n");
            break;
        case ELF_SECTION_TYPE_HASH:
            printf("Symbol hash table\n");
            break;
        case ELF_SECTION_TYPE_DYNAMIC:
            printf("Dynamic linking information\n");
            break;
        case ELF_SECTION_TYPE_NOTE:
            printf("Notes\n");
            break;
        case ELF_SECTION_TYPE_NOBITS:
            printf("Program space with no data (bss)\n");
            break;
        case ELF_SECTION_TYPE_REL:
            printf("Relocation entries without explicit addends\n");
            break;
        case ELF_SECTION_TYPE_SHLIB:
            printf("Reserved\n");
            break;
        case ELF_SECTION_TYPE_DYNSYM:
            printf("Dynamic linker symbol table\n");
            break;
        case ELF_SECTION_TYPE_INIT_ARRAY:
            printf("Array of constructors\n");
            break;
        case ELF_SECTION_TYPE_FINI_ARRAY:
            printf("Array of destructors\n");
            break;
        case ELF_SECTION_TYPE_PREINIT_ARRAY:
            printf("Array of pre-constructors\n");
            break;
        case ELF_SECTION_TYPE_GROUP:
            printf("Section group\n");
            break;
        case ELF_SECTION_TYPE_SYMTAB_SHNDX:
            printf("Extended section indices\n");
            break;
        case ELF_SECTION_TYPE_NUM:
            printf("Number of defined types\n");
            break;
        case ELF_SECTION_TYPE_LOOS:
            printf("Start OS-specific\n");
            break;
        default:
            printf("Unknown\n");
            break;
        }

        printf("Flags: %u (hex: %016llx)\n", sectionheaders[i].sh_flags,
                                            sectionheaders[i].sh_flags);
        printf("Virtual address: %016llx\n", sectionheaders[i].sh_addr);
        printf("Offset: %016llx\n", sectionheaders[i].sh_offset);
        printf("Size: %016llx\n", sectionheaders[i].sh_size);
        printf("Link: %u\n", sectionheaders[i].sh_link);
        printf("Info: %u\n", sectionheaders[i].sh_info);
        printf("Address alignment: %016llx\n", sectionheaders[i].sh_addralign);
        printf("Entry size (if section holds table): %016llx\n", sectionheaders[i].sh_entsize);
    }

    fclose(f);
    free(header);
    free(programheaders);
    free(sectionheaders);
}
