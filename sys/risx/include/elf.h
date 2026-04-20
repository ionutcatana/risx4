#ifndef ELF_H
#define ELF_H 1

#include <stdint.h>

// straight from wikipedia
// https://en.wikipedia.org/wiki/Executable_and_Linkable_Format

#define ELF_IDENTIFIER_MAGIC         { 0x7F, 'E', 'L', 'F' }

#define ELF_IDENTIFIER_CLASS_32_BIT  ((uint8_t)1)
#define ELF_IDENTIFIER_CLASS_64_BIT  ((uint8_t)2)

#define ELF_IDENTIFIER_DATA_LE       ((uint8_t)1)
#define ELF_IDENTIFIER_DATA_BE       ((uint8_t)2)

#define ELF_IDENTIFIER_VERSION       ((uint8_t)1)

#define ELF_IDENTIFIER_OSABI_SYSV    ((uint8_t)0x00) // i only included the ones
#define ELF_IDENTIFIER_OSABI_NETBSD  ((uint8_t)0x02) // that i cared about
#define ELF_IDENTIFIER_OSABI_LINUX   ((uint8_t)0x03)
#define ELF_IDENTIFIER_OSABI_GNUHURD ((uint8_t)0x04)
#define ELF_IDENTIFIER_OSABI_SOLARIS ((uint8_t)0x06)
#define ELF_IDENTIFIER_OSABI_AIX     ((uint8_t)0x07)
#define ELF_IDENTIFIER_OSABI_FREEBSD ((uint8_t)0x09)
#define ELF_IDENTIFIER_OSABI_OPENBSD ((uint8_t)0x0C)

#define ELF_IDENTIFIER_TYPE_NONE     ((uint16_t)0x00)
#define ELF_IDENTIFIER_TYPE_REL      ((uint16_t)0x01)
#define ELF_IDENTIFIER_TYPE_EXEC     ((uint16_t)0x02)
#define ELF_IDENTIFIER_TYPE_DYN      ((uint16_t)0x03)
#define ELF_IDENTIFIER_TYPE_CORE     ((uint16_t)0x04)
#define ELF_IDENTIFIER_TYPE_LOOS     ((uint16_t)0xfe00)
#define ELF_IDENTIFIER_TYPE_HIOS     ((uint16_t)0xfefe)
#define ELF_IDENTIFIER_TYPE_LOPROC   ((uint16_t)0xff00)
#define ELF_IDENTIFIER_TYPE_HIPROC   ((uint16_t)0xffff)

#define ELF_IDENTIFIER_MACHINE_NONE  ((uint16_t)0x00) // i only included modern,
#define ELF_IDENTIFIER_MACHINE_AMD64 ((uint16_t)0x3e) // common architectures
#define ELF_IDENTIFIER_MACHINE_ARM64 ((uint16_t)0xb7)
#define ELF_IDENTIFIER_MACHINE_RISCV ((uint16_t)0xf3)

struct elfheader {
    uint8_t ei_magic[4];
    uint8_t ei_class;
    uint8_t ei_data;
    uint8_t ei_version;
    uint8_t ei_osabi;
    uint8_t ei_abiversion;
    uint8_t ei_padding[7];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} __attribute__((packed));

#define ELF_PROGRAM_TYPE_NULL    ((uint32_t)0x00000000)
#define ELF_PROGRAM_TYPE_LOAD    ((uint32_t)0x00000001)
#define ELF_PROGRAM_TYPE_DYNAMIC ((uint32_t)0x00000002)
#define ELF_PROGRAM_TYPE_INTERP  ((uint32_t)0x00000003)
#define ELF_PROGRAM_TYPE_NOTE    ((uint32_t)0x00000004)
#define ELF_PROGRAM_TYPE_SHLIB   ((uint32_t)0x00000005)
#define ELF_PROGRAM_TYPE_PHDR    ((uint32_t)0x00000006)
#define ELF_PROGRAM_TYPE_TLS     ((uint32_t)0x00000007)
#define ELF_PROGRAM_TYPE_LOOS    ((uint32_t)0x60000000)
#define ELF_PROGRAM_TYPE_HIOS    ((uint32_t)0x6FFFFFFF)
#define ELF_PROGRAM_TYPE_LOPROC  ((uint32_t)0x70000000)
#define ELF_PROGRAM_TYPE_HIPROC  ((uint32_t)0x7FFFFFFF)

#define ELF_PROGRAM_FLAG_EXECUTE ((uint32_t)0x1)
#define ELF_PROGRAM_FLAG_WRITE   ((uint32_t)0x2)
#define ELF_PROGRAM_FLAG_READ    ((uint32_t)0x4)

struct elfprogramheader {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} __attribute__((packed));

#define ELF_SECTION_TYPE_NULL          ((uint32_t)0x0)
#define ELF_SECTION_TYPE_PROGBITS      ((uint32_t)0x1)
#define ELF_SECTION_TYPE_SYMTAB        ((uint32_t)0x2)
#define ELF_SECTION_TYPE_STRTAB        ((uint32_t)0x3)
#define ELF_SECTION_TYPE_RELA          ((uint32_t)0x4)
#define ELF_SECTION_TYPE_HASH          ((uint32_t)0x5)
#define ELF_SECTION_TYPE_DYNAMIC       ((uint32_t)0x6)
#define ELF_SECTION_TYPE_NOTE          ((uint32_t)0x7)
#define ELF_SECTION_TYPE_NOBITS        ((uint32_t)0x8)
#define ELF_SECTION_TYPE_REL           ((uint32_t)0x9)
#define ELF_SECTION_TYPE_SHLIB         ((uint32_t)0xA)
#define ELF_SECTION_TYPE_DYNSYM        ((uint32_t)0xB)
#define ELF_SECTION_TYPE_INIT_ARRAY    ((uint32_t)0xE)
#define ELF_SECTION_TYPE_FINI_ARRAY    ((uint32_t)0xF)
#define ELF_SECTION_TYPE_PREINIT_ARRAY ((uint32_t)0x10)
#define ELF_SECTION_TYPE_GROUP         ((uint32_t)0x11)
#define ELF_SECTION_TYPE_SYMTAB_SHNDX  ((uint32_t)0x12)
#define ELF_SECTION_TYPE_NUM           ((uint32_t)0x13)
#define ELF_SECTION_TYPE_LOOS          ((uint32_t)0x60000000)

#define ELF_SECTION_FLAG_WRITE            ((uint64_t)0x1)
#define ELF_SECTION_FLAG_ALLOC            ((uint64_t)0x2)
#define ELF_SECTION_FLAG_EXECINSTR        ((uint64_t)0x4)
#define ELF_SECTION_FLAG_MERGE            ((uint64_t)0x10)
#define ELF_SECTION_FLAG_STRINGS          ((uint64_t)0x20)
#define ELF_SECTION_FLAG_INFO_LINK        ((uint64_t)0x40)
#define ELF_SECTION_FLAG_LINK_ORDER       ((uint64_t)0x80)
#define ELF_SECTION_FLAG_OS_NONCONFORMING ((uint64_t)0x100)
#define ELF_SECTION_FLAG_GROUP            ((uint64_t)0x200)
#define ELF_SECTION_FLAG_TLS              ((uint64_t)0x400)
#define ELF_SECTION_FLAG_MASKOS           ((uint64_t)0x0FF00000)
#define ELF_SECTION_FLAG_MASKPROC         ((uint64_t)0xF0000000)
#define ELF_SECTION_FLAG_ORDERED          ((uint64_t)0x4000000)
#define ELF_SECTION_FLAG_EXCLUDE          ((uint64_t)0x8000000)

struct elfsectionheader {
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
} __attribute__((packed));

#endif
