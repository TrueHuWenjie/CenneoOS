// Cenneo OS
// /kernel/drivers/acpi/acpi.c
// Root System Description Pointer for ACPI

#include <lib/string.h>
#include <stddef.h>
#include <kmm.h>
#include <kls.h>
#include <types.h>
#include <acpi.h>

char *mem_main_bios;
char *mem_rsdt;

// Root System Description Pointer
union acpi_rsdp_descriptor *rsdp;
struct acpi_rsdt *rsdt;

// Find any kind of System Descriptor Tables
// #Notice: In this place we asuming that all the sdts are located
// at the same 4KB physical page with the RSDT.
struct acpi_sdt_header *acpi_sdt_locate(const char *signature)
{
    struct acpi_sdt_header *header;
    int entries = ACPI_RSDT_LIST_SIZE;
    
    for (int i = 0; i < entries; i ++)
    {
        header = (struct acpi_sdt_header *) ((uint32_t)mem_rsdt + (rsdt->sdt_list[i] & 0xfff));

        if(!strncmp(header->Signature, signature, 4))
            return header;
    }

    // No such SDT
    return NULL;
}

// Find the RSDP
void acpi_rsdp_locate(void)
{
    mem_main_bios = vmalloc(0xfffff - 0xe0000, VM_MAPD + VM_WAIT);
    vmap((void *)0xe0000, mem_main_bios, 0xfffff - 0xe0000);
    char *p;

    for (p = mem_main_bios; p < mem_main_bios + 0xfffff - 0xe0000; p ++)
    {
        if(!strncmp(p, "RSD PTR ", 8))
        {
            rsdp = (union acpi_rsdp_descriptor *) p;

            // Cheeck the revision of RSD

            // Checksum validation

            // Output
            if (rsdp->v10.revision == 0)
            {
                printk("RSDP revision:%d, OEM:%s\n", rsdp->v10.revision, rsdp->v10.oem_id);

                mem_rsdt = vmalloc(4096, VM_MAPD + VM_WAIT);
                vmap((void *)(rsdp->v10.rsdt_addr & 0xfffff000), mem_rsdt, 4096);

                rsdt = (struct acpi_rsdt *) ((rsdp->v10.rsdt_addr - \
                (uint32_t)(rsdp->v10.rsdt_addr & 0xfffff000)) + (uint32_t)mem_rsdt);

                printk("Mapping RSDT from physical address %#x to virtual address %#x\n", \
                rsdp->v10.rsdt_addr, rsdt);

                return;
            }
            else
            {
                error("Unsupported revision of RSDP.");
            }
            
        }
    }

    error("ACPI: No RSDP found!\n");
}
