#ifndef PRIVATE_ACPI_TYPES_H
#define PRIVATE_ACPI_TYPES_H 1

#include <stdint.h>

/* root system description pointer */
struct xsdp {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdpaddr;
    uint32_t length;
    uint64_t xsdtaddr;
    uint8_t xchecksum;
    uint8_t reserved[8];
} __attribute__((packed));

/* root table header */
struct sdtheader {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oemtableid[8];
    uint32_t oemrevision;
    uint32_t creatorid;
    uint32_t creatorrevision;
} __attribute__((packed));

/* fixed acpi description table */
struct genericaddress {
  uint8_t address_space;
  uint8_t bit_width;
  uint8_t bit_offset;
  uint8_t access_size;
  uint64_t address;
} __attribute__((packed));

struct fadt {
    struct sdtheader header;
    uint32_t firmware_ctrl;
    uint32_t dsdt_addr;
    uint8_t  reserved; // field used in acpi 1.0
    uint8_t  preferred_power_management_profile;
    uint16_t sci_interrupt;
    uint32_t smi_commandport;
    uint8_t  acpi_enable;
    uint8_t  acpi_disable;
    uint8_t  s4bios_req;
    uint8_t  pstate_control;
    uint32_t pm1_aevent_block;
    uint32_t pm1_bevent_block;
    uint32_t pm1_acontrol_block;
    uint32_t pm1_bcontrol_block;
    uint32_t pm2_control_block;
    uint32_t pm_timer_block;
    uint32_t gpe0_block;
    uint32_t gpe1_block;
    uint8_t  pm1_event_length;
    uint8_t  pm1_control_length;
    uint8_t  pm2_control_length;
    uint8_t  pm_timer_length;
    uint8_t  gpe0_length;
    uint8_t  gpe1_length;
    uint8_t  gpe1_base;
    uint8_t  cstate_control;
    uint16_t worst_c2_latency;
    uint16_t worst_c3_latency;
    uint16_t flush_size;
    uint16_t flush_stride;
    uint8_t  duty_offset;
    uint8_t  duty_width;
    uint8_t  day_alarm;
    uint8_t  month_alarm;
    uint8_t  century;
    uint16_t boot_architecture_flags; // reserved in acpi 1.0; used since acpi 2.0+
    uint8_t  reserved2;
    uint32_t flags;
    struct genericaddress reset_reg;
    uint8_t  reset_value;
    uint8_t  reser_ved3[3];
    uint64_t x_firmwarecontrol; // 64bit pointers - available on acpi 2.0+
    uint64_t x_dsdt;
    struct genericaddress x_pm1_aevent_block;
    struct genericaddress x_pm1_bevent_block;
    struct genericaddress x_pm1_acontrol_block;
    struct genericaddress x_pm1_bcontrol_block;
    struct genericaddress x_pm2_control_block;
    struct genericaddress x_pm_timer_block;
    struct genericaddress x_gpe0_block;
    struct genericaddress x_gpe1_block;
} __attribute__((packed));

/* multiple apic description table */
struct madtheader {
    struct sdtheader header;
    uint32_t local_apic_addr;
    uint32_t flags;
} __attribute__((packed));

struct madtrecordheader {
    uint8_t entry_type;
    uint8_t record_length;
} __attribute__((packed));

// represents a single logical processor and its local interrupt controller.
struct record_lapic {
    uint8_t acpi_processor_id;
    uint8_t acpi_id;
    uint32_t flags;
} __attribute__((packed));

// represents a i/o apic.
struct record_io_apic {
    uint8_t io_apic_id;
    uint8_t reserved; // = 0
    uint32_t io_apic_address;
    uint32_t global_system_interrupt_base;
} __attribute__((packed));

struct record_io_apic_iso {
    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t global_system_interrupt;
    uint16_t flags;
} __attribute__((packed));

struct record_io_apic_nmis {
    uint8_t nmi_source;
    uint8_t reserved;
    uint16_t flags;
    uint32_t global_system_interrupt;
} __attribute__((packed));

struct record_io_lapic_nmi {
    uint8_t acpi_processord_id;
    uint16_t flags;
    uint8_t lint_number;
} __attribute__((packed));

struct record_io_lapic_ao {
    uint16_t reserved;
    uint64_t lapic_address;
} __attribute__((packed));

struct record_io_lx2apic {
    uint16_t reserved;
    uint32_t lx2apic_id;
    uint32_t flags;
    uint32_t acpi_id;
} __attribute__((packed));

#endif
