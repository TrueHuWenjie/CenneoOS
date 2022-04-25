// Cenneo OS
// /kernel/drivers/pci/pci.c
// Peripheral Component Interconnect support

#include <arch.h>
#include <kls.h>
#include <types.h>
#include <acpi.h>
#include <drivers/pci.h>

struct pci_mcfg_table *mcfg;



struct pci_mcfg_table *pci_mcfg_table;

int pci_config_write_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t reg, uint32_t data)
{
        uint32_t address;
        uint32_t lbus  = (uint32_t)bus;
        uint32_t lslot = (uint32_t)slot;
        uint32_t lfunc = (uint32_t)func;
    
        // Create configuration address
        address = (uint32_t)((lbus << 16) | (lslot << 11) |
                (lfunc << 8) | (reg * sizeof(uint32_t)) | ((uint32_t)0x80000000));
        
        // Write out the address
        io_out32(PCI_IO_CONFIG_ADDRESS, address);
        // Read in the data
        io_out32(PCI_IO_CONFIG_DATA, data);

        return 0;
}

uint32_t pci_config_read_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t reg)
{
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
 
    // Create configuration address
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (reg * sizeof(uint32_t)) | ((uint32_t)0x80000000));
 
    // Write out the address
    io_out32(PCI_IO_CONFIG_ADDRESS, address);
    // Read in the data
    return io_in32(PCI_IO_CONFIG_DATA);
}

uint16_t pci_config_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
 
    // Create configuration address
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
 
    // Write out the address
    io_out32(PCI_IO_CONFIG_ADDRESS, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = (uint16_t)((io_in32(PCI_IO_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}

// Get the Vendor ID, 0xffff return as if this device dosen't exist
uint16_t pci_get_vendor_id(uint8_t bus, uint8_t slot, uint8_t function)
{
    return pci_config_read(bus, slot, 0, 0x00);
}

// Get the Device ID
uint16_t pci_get_device_id(uint8_t bus, uint8_t slot, uint8_t function)
{
    return pci_config_read(bus, slot, 0, 0x02);
}

// Get the Header Type
uint8_t pci_get_header_type(uint8_t bus, uint8_t slot, uint8_t function)
{
    return pci_config_read(bus, slot, 0, 0x0e) & 0xff;
}

// Get the Class Code
uint8_t pci_get_class(uint8_t bus, uint8_t slot, uint8_t function)
{
    return (pci_config_read(bus, slot, 0, 0x0a) & 0xff00) >> 8;
}

// Get the Sub Class
uint8_t pci_get_sub_class(uint8_t bus, uint8_t slot, uint8_t function)
{
    return pci_config_read(bus, slot, 0, 0x0a) & 0xff;
}

void pci_check_function(uint8_t bus, uint8_t device, uint8_t function)
{
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class;
    uint8_t sub_class;
 
    vendor_id = pci_get_vendor_id(bus, device, function);
    device_id = pci_get_device_id(bus, device, function);
    class = pci_get_class(bus, device, function);

    // Output
    if (vendor_id != 0xffff)
        printk("PCI device Vendor ID:%#x, Device ID:%#x, Class:%#x, Sub Class:%#x.\n", vendor_id, device_id, class, sub_class);

    // Using correct drivers to initialize the hardware
    switch (class)
    {
        case PCI_CLASS_UNCLASSIFIED:
        case PCI_CLASS_MASS_STORAGE_CONTROLLER:
        case PCI_CLASS_NETWORK_CONTROLLER:
            pci_nic_init(bus, device, function);
            break;
        case PCI_CLASS_DISPLAY_CONTROLLER:
        case PCI_CLASS_MULTIMEDIA_CONTROLLER:
        case PCI_CLASS_MEMORY_CONTROLLER:
        case PCI_CLASS_BRIDGE:
        case PCI_CLASS_SIMPLE_COM_CONTROLLER:
        case PCI_CLASS_BASE_SYSTEM_PERIPHERAL:
        case PCI_CLASS_INPUT_DEVICE_CONTROLLER:
        case PCI_CLASS_PROCESSOR:
        case PCI_CLASS_SERIAL:
        case PCI_CLASS_WIRELESS:
        case PCI_CLASS_INTELLIGENT_CONTROLLER:
        default:
            break;
    }
}

void pci_check_device(uint8_t bus, uint8_t device)
{
    uint8_t function;

    pci_check_function(bus, device, 0);

    // Check if this device support multi-functions
    if (pci_get_header_type(bus, device, 0) & 0x80 == 0x80)
        for (function = 1; function < 8; function ++)
        {
            pci_check_function(bus, device, function);
        }
}

void pci_check_bus(uint8_t bus)
{
    uint8_t device;
 
    for (device = 0; device < 32; device ++)
        pci_check_device(bus, device);
}

void pci_check_all_buses(void)
{
    uint8_t function;
    uint8_t bus;
    uint8_t header_type;

    header_type = pci_get_header_type(0, 0, 0);
     if ((header_type & 0x80) == 0) {
         // Single PCI host controller
         pci_check_bus(0);
     } else {
         // Multiple PCI host controllers
         for (function = 0; function < 8; function ++) {
             if (pci_get_vendor_id(0, 0, function) != 0xFFFF) break;
             bus = function;
             pci_check_bus(bus);
         }
     }
}

void init_pci(void)
{
    // Show information
    printk("Initializing PCI bus...\n");

    mcfg = (struct pci_mcfg_table *)acpi_sdt_locate("MCFG");

    // Checking if there is MCFG table
    if (!mcfg)
    {
        // Unsupport MCFG table, using I/O
        pci_check_all_buses();   
    }
    else
    {
        // Support MCFG table, using MMIO
        
        // Dump the information of configuration space
        for (int i = 0; i < PCI_NUM_CONFIG_SPACE; i ++)
        {
            printk("Base Addr:%#x, Group Number:%d, Start:%d, End:%d\n",
                mcfg->pci_config_space[i].addr,
                mcfg->pci_config_space[i].group_num,
                mcfg->pci_config_space[i].start,
                mcfg->pci_config_space[i].end);
        }
    }
}
