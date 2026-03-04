#ifndef ACPI_H
#define ACPI_H 1

#include <stdint.h>

#define ACPI_VERSION_1 0            // 1.0
#define ACPI_VERSION_SUBSEQUENT 2   // 2.0-6.1

/* root system description pointer */
typedef struct {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdpaddr;
} __attribute__((packed)) rsdp_t;

typedef struct {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdpaddr;
    uint32_t length;
    uint64_t xsdtaddr;
    uint8_t xchecksum;
    uint8_t reserved[8];
} __attribute__((packed)) xsdp_t;

/* root table header */
typedef struct {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oemtableid[8];
    uint32_t oemrevision;
    uint32_t creatorid;
    uint32_t creatorrevision;
} __attribute__((packed)) sdtheader_t;

// typedef struct {
//     sdtheader_t header;
//     uint32_t sdtaddr[(header.length - sizeof(sdtheader_t)) / 4];
// } __attribute__((packed)) rsdt_t;

// typedef struct  {
//     sdtheader_t header;
//     uint64_t sdtaddr[(header.length - sizeof(sdtheader_t)) / 8];
// } __attribute__((packed)) xsdt_t;

/* fixed acpi description table */
typedef struct {
  uint8_t address_space;
  uint8_t bit_width;
  uint8_t bit_offset;
  uint8_t access_size;
  uint64_t address;
} __attribute__((packed)) genericaddress_t;

#define PREFERRED_UNSPECIFIED       ((uint8_t)0)
#define PREFERRED_DESKTOP           ((uint8_t)1)
#define PREFERRED_MOBILE            ((uint8_t)2)
#define PREFERRED_WORKSTATION       ((uint8_t)3)
#define PREFERRED_ENTERPRISE_SERVER ((uint8_t)4)
#define PREFERRED_SOHO_SERVER       ((uint8_t)5)
#define PREFERRED_APPLIANCE_PC      ((uint8_t)6)
#define PREFERRED_PERFORMANCE       ((uint8_t)7)

typedef struct {
    sdtheader_t header;
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
    genericaddress_t reset_reg;
    uint8_t  reset_value;
    uint8_t  reser_ved3[3];
    uint64_t x_firmwarecontrol; // 64bit pointers - available on acpi 2.0+
    uint64_t x_dsdt;
    genericaddress_t x_pm1_aevent_block;
    genericaddress_t x_pm1_bevent_block;
    genericaddress_t x_pm1_acontrol_block;
    genericaddress_t x_pm1_bcontrol_block;
    genericaddress_t x_pm2_control_block;
    genericaddress_t x_pm_timer_block;
    genericaddress_t x_gpe0_block;
    genericaddress_t x_gpe1_block;
} __attribute__((packed)) fadt_t;

/* multiple apic description table */
typedef struct {
    sdtheader_t header;
    uint32_t local_apic_addr;
    uint32_t flags;
} __attribute__((packed)) madtheader_t;

typedef struct {
    uint8_t entry_type;
    uint8_t record_length;
} __attribute__((packed)) madtrecordheader_t;

#define MADT_RECORD_LAPIC                             ((uint8_t)0)
#define MADT_RECORD_IO_APIC                           ((uint8_t)1)
#define MADT_RECORD_IO_APIC_INTERRUPT_SOURCE_OVERRIDE ((uint8_t)2)
#define MADT_RECORD_IO_APIC_NMI_SOURCE                ((uint8_t)3)
#define MADT_RECORD_LAPIC_NMI                         ((uint8_t)4)
#define MADT_RECORD_LAPIC_ADDRESS_OVERRIDE            ((uint8_t)5)
#define MADT_RECORD_LX2APIC                           ((uint8_t)9)
// typedef struct {

// } __attribute__((packed)) _t;
// https://wiki.osdev.org/MADT
#define LAPIC_FLAG_ENABLED ((uint8_t)0)
#define LAPIC_FLAG_ONLINE  ((uint8_t)1)
// represents a single logical processor and its local interrupt controller.
typedef struct {
    uint8_t acpi_processor_id;
    uint8_t acpi_id;
    uint32_t flags;
} __attribute__((packed)) record_lapic_t;

// represents a i/o apic.
typedef struct {
    uint8_t io_apic_id;
    uint8_t reserved; // = 0
    uint32_t io_apic_address;
    uint32_t global_system_interrupt_base;
} __attribute__((packed)) record_io_apic_t;

// #define MADT_RECORD_FLAG_POLARITY_NO_OVERRIDE (0)
// #define MADT_RECORD_FLAG_POLARITY_DEFAULT (0)
// #define MADT_RECORD_FLAG_POLARITY_ACTIVE_HIGH_OVERRIDE (1)
// #define MADT_RECORD_FLAG_POLARITY_ACTIVE_LOW_OVERRIDE (3)
// #define MADT_RECORD_FLAG_TRIGGER_MODE_NO_OVERRIDE (0)
// #define MADT_RECORD_FLAG_TRIGGER_MODE_DEFAULT (0)
// #define MADT_RECORD_FLAG_TRIGGER_MODE_EDGE_TRIGGERED (1)
// #define MADT_RECORD_FLAG_TRIGGER_MODE_LEVEL_TRIGGERED (3)
typedef struct {
    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t global_system_interrupt;
    uint16_t flags;
} __attribute__((packed)) record_io_apic_iso_t;

typedef struct {
    uint8_t nmi_source;
    uint8_t reserved;
    uint16_t flags;
    uint32_t global_system_interrupt;
} __attribute__((packed)) record_io_apic_nmis_t;

typedef struct {
    uint8_t acpi_processord_id;
    uint16_t flags;
    uint8_t lint_number;
} __attribute__((packed)) record_io_lapic_nmi_t;

typedef struct {
    uint16_t reserved;
    uint64_t lapic_address;
} __attribute__((packed)) record_io_lapic_ao_t;

typedef struct {
    uint16_t reserved;
    uint32_t lx2apic_id;
    uint32_t flags;
    uint32_t acpi_id;
} __attribute__((packed)) record_io_lx2apic_t;

// arch/x86/acpi.c
void initacpi(void);
int  acpiversion(void);
void validate_dsp(uint64_t dsp_address);
void validate_dst(uint64_t dst_address);

void parse_rdsp();
void parse_xdsp();
void parse_rdst();
void parse_xdst();

#endif
