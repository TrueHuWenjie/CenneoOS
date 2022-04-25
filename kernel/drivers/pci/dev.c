// Cenneo OS
// /kernel/drivers/pci/dev.c
// Network

#include <types.h>
#include <drivers/pci.h>
#include <drivers/virtio.h>
#include <drivers/rtl8139.h>

#define PCI_NIC_ETH 0x00        // Ethernet Controller

void pci_nic_init(uint8_t bus, uint8_t device, uint8_t function)
{
    uint16_t vendor_id;
    uint8_t sub_class;

    vendor_id = pci_get_vendor_id(bus, device, function);
    sub_class = pci_get_sub_class(bus, device, function);
    switch (sub_class)
    {
        case PCI_NIC_ETH:
            // Virtio
            switch (vendor_id)
            {
                case 0x1af4:
                    virtio_net_init(bus, device, function);
                    break;
                case 0x10ec:
                    rtl8139_init(bus, device, function);
                    break;
                default:
                    break;
            }
        default:
            break;
    }
}
