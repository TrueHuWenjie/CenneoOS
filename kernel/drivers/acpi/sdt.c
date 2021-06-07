// Cenneo OS
// /kernel/drivers/acpi/acpi.c
// Root System Description Pointer for ACPI

#include <lib/string.h>
#include <stddef.h>
#include <kmm.h>
#include <kls.h>
#include <types.h>

char *main_bios;

union acpi_rsdp_descriptor *rsdp;

// Root System Description Pointer

#pragma pack(push)
#pragma pack(1)

// ACPI Version 1.0
struct acpi_rsdp_descriptor_10
{
    char        signature[8];
    uint8_t     checksum;
    char        oem_id[6];
    uint8_t     revision;
    uint32_t    rsdt_addr;
};

// ACPI Version 2.0
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

// Find the RSDP
void acpi_rsdp_locate(void)
{
    main_bios = vmalloc(0xfffff - 0xe0000, VM_MAPD + VM_WAIT);
    vmap(0xe0000, main_bios, 0xfffff - 0xe0000);
    char *p;

    for (p = main_bios; p < main_bios + 0xfffff - 0xe0000; p ++)
    {
        if(!strncmp(p, "RSD PTR ", 8))
        {
            rsdp = (union acpi_rsdp_descriptor *) p;

            // Checksum validation

            return;
        }
    }

    error("ACPI: No RSDP found!\n");
}
