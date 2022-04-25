// Cenneo OS
// /kernel/drivers/network/rtl8139/rtl8139.c
// RTL8139 Support

#include <arch.h>
#include <types.h>
#include <kls.h>
#include <kmm.h>
#include <mmu.h>
#include <drivers/pci.h>
#include <drivers/rtl8139.h>

struct rtl8139_config
{
    uint32_t io_base;
} rtl8139_config;

// IO
#define RTL8139_IO_MAC(i)   (0x00 + i)
#define RTL8139_IO_MAR(i)   (0x08 + i)
#define RTL8139_IO_RBSTART  0x30
#define RTL8139_IO_CMD      0x37
#define RTL8139_IO_IMR      0x3c
#define RTL8139_IO_ISR      0x3e
#define RTL8139_IO_TXCONFIG 0x40
#define RTL8139_IO_RXCONFIG 0x44
#define RTL8139_IO_CONFIG_0 0x51
#define RTL8139_IO_CONFIG_1 0x52

// Bits in IO port of RTL8139_IO_IMR
#define RTL8139_IMR_RXOK        0x0001
#define RTL8139_IMR_RXERR       0x0002
#define RTL8139_IMR_TXOK        0x0004
#define RTL8139_IMR_TXERR       0x0008
#define RTL8139_IMR_RXOF        0x0010
#define RTL8139_IMR_RXUNDERRUN  0x0020
#define RTL8139_IMR_RXFIFOOVER  0x0040
#define RTL8139_IMR_PCSTIMEOUT  0x4000
#define RTL8139_IMR_PCIERR      0x8000

// Bits in IO port of RTL8139_IO_RXCONFIG
#define RTL8139_RXCONFIG_ACCEPT_ALLPHYS     0x01
#define RTL8139_RXCONFIG_ACCEPT_MYPHYS      0x02
#define RTL8139_RXCONFIG_ACCEPT_MULTICAST   0x04
#define RTL8139_RXCONFIG_ACCEPT_BROADCAST   0x08
#define RTL8139_RXCONFIG_ACCEPT_RUNT        0x10
#define RTL8139_RXCONFIG_ACCEPT_ERROR       0x20
#define RTL8139_RXCONFIG_ACCEPT_ERROR       0x20
#define RTL8139_RXCONFIG_WRAP               0x80

void irq_rtl8139_handle(void)
{
    printk("irq rtl8139:%#x.\n", io_in16(rtl8139_config.io_base + RTL8139_IO_ISR));
    io_out16(rtl8139_config.io_base + RTL8139_IO_ISR, 0x00);
}

void rtl8139_init(uint8_t bus, uint8_t device, uint8_t function)
{
    uint32_t bar;
    uint16_t cmd;
    void * rx_buffer;

    printk("RTL 8139 Network Controller\n");

    // Get the base IO address
    for (int i = 4; i < 9; i ++)
    {
        bar = pci_config_read_dword(bus, device, function, i);

        if (bar & 0x01)
        {
            rtl8139_config.io_base = bar & 0xfffffffc;
            break;
        }
    }

    // Enable PCI Bus Mastering for this device
    cmd = pci_config_read_dword(bus, device, function, 1);
    pci_config_write_dword(bus, device, function, 1, cmd | 0x04);

    // Set the LWAKE + LWPTN to active high
    io_out8(rtl8139_config.io_base + RTL8139_IO_CONFIG_1, 0x0);

    // Software Reset and check
    io_out8(rtl8139_config.io_base + RTL8139_IO_CMD, 0x10);
    while ((io_in8(rtl8139_config.io_base + RTL8139_IO_CMD) & 0x10) != 0);

    // Set the memory buffer
    rx_buffer = vmalloc(8192 + 16, VM_CTND | VM_WAIT);
    io_out32(rtl8139_config.io_base + RTL8139_IO_RBSTART, (u32)paging_virt2phys(rx_buffer));

    // Set IMR + ISR(Transmit OK and Receive OK)
    io_out16(rtl8139_config.io_base + RTL8139_IO_IMR, RTL8139_IMR_RXOK | RTL8139_IMR_TXOK);


    io_out32(rtl8139_config.io_base + RTL8139_IO_RXCONFIG, \
             RTL8139_RXCONFIG_ACCEPT_ALLPHYS   | 
             RTL8139_RXCONFIG_ACCEPT_MYPHYS    | 
             RTL8139_RXCONFIG_ACCEPT_MULTICAST | 
             RTL8139_RXCONFIG_ACCEPT_BROADCAST);
    
    register_PIC(11, &irq_rtl8139_handle, "RTL8139 Network Controller");

    // Open
    io_out8(rtl8139_config.io_base + RTL8139_IO_CMD, 0x0C); // Sets the RE and TE bits high
}
