// Cenneo OS
// /kernel/include/drivers/pci.h
// Peripheral Component Interconnect support

#ifndef PCI_H_
#define PCI_H_

#include <types.h>
#include <acpi.h>

// Vendor ID information

// The format of 32-bit port PCI_IO_CONFIG_ADDRESS:
// Bit 31       Bits 30-24  Bits 23-16  Bits 15-11      Bits 10-8       Bits 7-0
// Enable Bit	Reserved	Bus Number	Device Number	Function Number	Register Offset
#define PCI_IO_CONFIG_ADDRESS 0xcf8
#define PCI_IO_CONFIG_DATA    0xcfc

#define PCI_CLASS_UNCLASSIFIED 0x00
#define PCI_CLASS_MASS_STORAGE_CONTROLLER 0x01
#define PCI_CLASS_NETWORK_CONTROLLER 0x02
#define PCI_CLASS_DISPLAY_CONTROLLER 0x03
#define PCI_CLASS_MULTIMEDIA_CONTROLLER 0x04
#define PCI_CLASS_MEMORY_CONTROLLER 0x05
#define PCI_CLASS_BRIDGE 0x06
#define PCI_CLASS_SIMPLE_COM_CONTROLLER 0x07
#define PCI_CLASS_BASE_SYSTEM_PERIPHERAL 0x08
#define PCI_CLASS_INPUT_DEVICE_CONTROLLER 0x09
#define PCI_CLASS_PROCESSOR 0x0a
#define PCI_CLASS_SERIAL 0x0b
#define PCI_CLASS_WIRELESS 0x0c
#define PCI_CLASS_INTELLIGENT_CONTROLLER 0x0d

#pragma pack(push)
#pragma pack(1)

struct pci_config_space
{
    uint64_t addr;
    uint16_t group_num;
    uint8_t start;
    uint8_t end;
    uint8_t rsvd[4];
};

// MCFG
#define PCI_NUM_CONFIG_SPACE ((mcfg->header.Length - sizeof(mcfg->header)) / sizeof(struct pci_config_space))
struct pci_mcfg_table
{
    struct acpi_sdt_header header;
    uint8_t rsvd[8];
    struct pci_config_space *pci_config_space;
};

#pragma pack(pop)

int pci_config_write_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t reg, uint32_t data);

uint32_t pci_config_read_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t reg);

// Get the Vendor ID, 0xffff return as if this device dosen't exist
uint16_t pci_get_vendor_id(uint8_t bus, uint8_t slot, uint8_t function);

// Get the Device ID
uint16_t pci_get_device_id(uint8_t bus, uint8_t slot, uint8_t function);

// Get the Header Type
uint8_t pci_get_header_type(uint8_t bus, uint8_t slot, uint8_t function);

// Get the Class Code
uint8_t pci_get_class(uint8_t bus, uint8_t slot, uint8_t function);

// Get the Sub Class
uint8_t pci_get_sub_class(uint8_t bus, uint8_t slot, uint8_t function);

void init_pci(void);

void pci_nic_init(uint8_t bus, uint8_t device, uint8_t function);

#endif
