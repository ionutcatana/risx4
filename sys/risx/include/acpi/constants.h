#ifndef PRIVATE_ACPI_CONSTANTS_H
#define PRIVATE_ACPI_CONSTANTS_H 1

/* constants used accross all acpi tables                                     */
#define ACPI_VERSION_1                                ((uint64_t)0) // 1.0
#define ACPI_VERSION_SUBSEQUENT                       ((uint64_t)2) // 2.0-6.1

/* SDT signature constants (4 bytes, not null-terminated)                     */
#define SDT_SIG_FACP "FACP"
#define SDT_SIG_APIC "APIC"
#define SDT_SIG_DSDT "DSDT"
#define SDT_SIG_SRAT "SRAT"
#define SDT_SIG_XSDT "XSDT"

/* FADT preferred power management profile                                    */
#define PREFERRED_UNSPECIFIED                         ((uint8_t)0)
#define PREFERRED_DESKTOP                             ((uint8_t)1)
#define PREFERRED_MOBILE                              ((uint8_t)2)
#define PREFERRED_WORKSTATION                         ((uint8_t)3)
#define PREFERRED_ENTERPRISE_SERVER                   ((uint8_t)4)
#define PREFERRED_SOHO_SERVER                         ((uint8_t)5)
#define PREFERRED_APPLIANCE_PC                        ((uint8_t)6)
#define PREFERRED_PERFORMANCE                         ((uint8_t)7)

/* MADT record types                                                          */
#define MADT_RECORD_LAPIC                             ((uint8_t)0)
#define MADT_RECORD_IO_APIC                           ((uint8_t)1)
#define MADT_RECORD_IO_APIC_INTERRUPT_SOURCE_OVERRIDE ((uint8_t)2)
#define MADT_RECORD_IO_APIC_NMI_SOURCE                ((uint8_t)3)
#define MADT_RECORD_LAPIC_NMI                         ((uint8_t)4)
#define MADT_RECORD_LAPIC_ADDRESS_OVERRIDE            ((uint8_t)5)
#define MADT_RECORD_LX2APIC                           ((uint8_t)9)

/* LAPIC flags (bit positions)                                                */
#define LAPIC_FLAG_ENABLED                            ((uint32_t)(1 << 0))
#define LAPIC_FLAG_ONLINE                             ((uint32_t)(1 << 1))

/* MADT interrupt source override flags                                       */
#define MADT_FLAG_POLARITY_MASK                       ((uint16_t)0x0003)
#define MADT_FLAG_POLARITY_CONFORMS                   ((uint16_t)0x0000)
#define MADT_FLAG_POLARITY_ACTIVE_HIGH                ((uint16_t)0x0001)
#define MADT_FLAG_POLARITY_ACTIVE_LOW                 ((uint16_t)0x0003)
#define MADT_FLAG_TRIGGER_MASK                        ((uint16_t)0x000c)
#define MADT_FLAG_TRIGGER_CONFORMS                    ((uint16_t)0x0000)
#define MADT_FLAG_TRIGGER_EDGE                        ((uint16_t)0x0004)
#define MADT_FLAG_TRIGGER_LEVEL                       ((uint16_t)0x000c)

/* MADT flags (in madt header)                                                */
#define MADT_FLAG_PCAT_COMPAT                         ((uint32_t)(1 << 0))

/* SRAT record types                                                          */
#define SRAT_RECORD_LAPIC_AFFINITY                    ((uint8_t)0)
#define SRAT_RECORD_MEMORY_AFFINITY                   ((uint8_t)1)
#define SRAT_RECORD_LX2APIC_AFFINITY                  ((uint8_t)2)

/* SRAT memory affinity flags                                                 */
#define SRAT_MEM_FLAG_ENABLED                         ((uint32_t)(1 << 0))
#define SRAT_MEM_FLAG_HOTPLUGGABLE                    ((uint32_t)(1 << 1))
#define SRAT_MEM_FLAG_NONVOLATILE                     ((uint32_t)(1 << 2))

/* SRAT LAPIC affinity flags                                                  */
#define SRAT_LAPIC_FLAG_ENABLED                       ((uint32_t)(1 << 0))

#endif
