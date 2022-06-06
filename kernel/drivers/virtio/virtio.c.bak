// Cenneo OS
// /kernel/drivers/virtio/virtio.c
// Virtio Network Controller Support

#include <types.h>
#include <kls.h>
#include <arch.h>
#include <drivers/pci.h>
#include <drivers/virtio.h>

uint32_t virtio_net_io_base;

// Driver-read-only configuration fields
#define VIRTIO_NET_S_LINK_UP     1
#define VIRTIO_NET_S_ANNOUNCE    2
struct virtio_net_config
{
    uint8_t mac[6];
    uint16_t status;
    uint16_t max_virtqueue_pairs;
    uint16_t mtu;
} virtio_net_config;

void virtio_net_init(uint8_t bus, uint8_t device, uint8_t function)
{
    uint32_t bar;
    uint32_t dev_f;
    uint8_t dev_s;

    printk("Virtio Network Controller\n");

    // Get the base IO address
    for (int i = 4; i < 9; i ++)
    {
        bar = pci_config_read_dword(bus, device, function, i);

        if (bar & 0x01)
        {
            virtio_net_io_base = bar & 0xfffffffc;
            //printk("io base:%#x", virtio_net_io_base);
            break;
        }
    }

    // Reset the device
    io_out8(virtio_net_io_base + VIRTIO_IO_REG_DEVICE_STATUS, 0);

    // Set the Acknowledge status bit
    dev_s = io_in8(virtio_net_io_base + VIRTIO_IO_REG_DEVICE_STATUS);
    io_out8(virtio_net_io_base + VIRTIO_IO_REG_DEVICE_STATUS, dev_s | VIRTIO_DEVICE_S_ACKNOWLEDGED);
    

    // Set the Driver status bit

    // Fetching Device Features
    dev_f = io_in32(virtio_net_io_base + VIRTIO_IO_REG_DEVICE_FEATURES);
    printk("Device Features:%#x.\n", dev_f);

    // Set FEATURES_OK Status bit, the device will not accept any feature bit after this step

    // Re-read to verify if the FEATURES_OK is still set



    // MAC Address
    // Checking if MAC given by the device
    if (dev_f && VIRTIO_NET_F_MAC)
    {
        printk("MAC given\n");
    }else{
        printk("MAC not given\n");
    }
    printk("MAC Address: ");
    for (int j = 0; j < 6; j ++)
    {
        virtio_net_config.mac[j] = io_in8(virtio_net_io_base + VIRTIO_NET_IO_REG_MAC1 + j);
        printk("%02X", virtio_net_config.mac[j]);
        if (j < 5) printk(":");
    }
    printk("\n");

    if (dev_f && VIRTIO_NET_F_MQ) virtio_net_config.max_virtqueue_pairs = 1;
    if (dev_f && VIRTIO_NET_F_MTU) virtio_net_config.mtu = 4096;

    // Set the DRIVER_OK status bit

}


