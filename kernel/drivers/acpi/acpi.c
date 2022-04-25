// Cenneo OS
// /kernel/drivers/acpi/acpi.c
// Advanced Configuration and Power Interface

// Root System Description Pointer

void init_acpi(void)
{
    printk("Initializing ACPI...\n");
    printk("Finding RSDP...\n");
	acpi_rsdp_locate();
}
