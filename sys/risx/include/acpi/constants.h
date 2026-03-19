#ifndef PRIVATE_ACPI_CONSTANTS_H
#define PRIVATE_ACPI_CONSTANTS_H 1

/* constants used accross all acpi tables                                     */
#define ACPI_VERSION_1                                ((uint64_t)0) // 1.0
#define ACPI_VERSION_SUBSEQUENT                       ((uint64_t)2) // 2.0-6.1

#define PREFERRED_UNSPECIFIED                         ((uint8_t)0)
#define PREFERRED_DESKTOP                             ((uint8_t)1)
#define PREFERRED_MOBILE                              ((uint8_t)2)
#define PREFERRED_WORKSTATION                         ((uint8_t)3)
#define PREFERRED_ENTERPRISE_SERVER                   ((uint8_t)4)
#define PREFERRED_SOHO_SERVER                         ((uint8_t)5)
#define PREFERRED_APPLIANCE_PC                        ((uint8_t)6)
#define PREFERRED_PERFORMANCE                         ((uint8_t)7)

#define MADT_RECORD_LAPIC                             ((uint8_t)0)
#define MADT_RECORD_IO_APIC                           ((uint8_t)1)
#define MADT_RECORD_IO_APIC_INTERRUPT_SOURCE_OVERRIDE ((uint8_t)2)
#define MADT_RECORD_IO_APIC_NMI_SOURCE                ((uint8_t)3)
#define MADT_RECORD_LAPIC_NMI                         ((uint8_t)4)
#define MADT_RECORD_LAPIC_ADDRESS_OVERRIDE            ((uint8_t)5)
#define MADT_RECORD_LX2APIC                           ((uint8_t)9)
// https://wiki.osdev.org/MADT
#define LAPIC_FLAG_ENABLED                            ((uint8_t)0)
#define LAPIC_FLAG_ONLINE                             ((uint8_t)1)

#endif
