// Cenneo OS
// /kernel/drivers/acpi/acpi.h
// Advanced Configuration and Power Interface

#ifndef ACPI_H_
#define ACPI_H_

#include <types.h>

extern struct acpi_rsdt *rsdt;

#pragma pack(push)
#pragma pack(1)

// The header of any kind of SDT(System Description Table)
struct acpi_sdt_header {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
};

// 
#define ACPI_XSDT_LIST_SIZE ((header.Length - sizeof(header)) / sizeof(uint32_t))
struct acpi_xsdt {
  struct acpi_sdt_header header;
  uint64_t *sdt_list;
};

// ACPI Root System Description Table
#define ACPI_RSDT_LIST_SIZE ((rsdt->header.Length - sizeof(rsdt->header)) / sizeof(uint32_t))
struct acpi_rsdt {
  struct acpi_sdt_header header;
  uint32_t sdt_list[];
};

// RSDP structure for ACPI Version 1.0
struct acpi_rsdp_descriptor_10
{
    char        signature[8];
    uint8_t     checksum;
    char        oem_id[6];
    uint8_t     revision;
    uint32_t    rsdt_addr;
};

// Extensions after RSDP structure only after ACPI Version 2.0
struct acpi_rsdp_descriptor_20
{
    struct acpi_rsdp_descriptor_10 first_part;

    uint32_t    Length;
    uint64_t    XsdtAddress;
    uint8_t     ExtendedChecksum;
    uint8_t     reserved[3];
};

// ACPI Root System Description Pointer
union acpi_rsdp_descriptor
{
    struct acpi_rsdp_descriptor_10 v10;
    struct acpi_rsdp_descriptor_20 v20;
};

#pragma pack(pop)

// Find any kind of System Descriptor Tables
// #Notice: In this place we asuming that all the sdts are located
// at the same 4KB physical page with the RSDT.
struct acpi_sdt_header *acpi_sdt_locate(const char *signature);

#endif
