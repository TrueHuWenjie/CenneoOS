// Cenneo OS
// /kernel/include/drivers/virtio.h
// Virtio support

#ifndef VIRTIO_H_
#define VIRTIO_H_

#include <types.h>

// VirtIO Network Packet header
struct virtio_packet_header
{
    uint8_t  Flags;                 // Bit 0: Needs checksum; Bit 1: Received packet has valid data;
                                    // Bit 2: If VIRTIO_NET_F_RSC_EXT was negotiated, the device processes
                                    // duplicated ACK segments, reports number of coalesced TCP segments in ChecksumStart
                                    // field and number of duplicated ACK segments in ChecksumOffset field,
                                    // and sets bit 2 in Flags(VIRTIO_NET_HDR_F_RSC_INFO) 
    uint8_t  SegmentationOffload;   // 0:None 1:TCPv4 3:UDP 4:TCPv6 0x80:ECN
    uint16_t HeaderLength;          // Size of header to be used durlng segmentation.
    uint16_t SegmentLength;         // Maximum segment size (not including header).
    uint16_t ChecksumStart;         // The position to begin calculating the checksum.
    uint16_t ChecksumOffset;        // The position after ChecksumStart to store the checksum.
    uint16_t BufferCount;           // Used when merging buffers.
};

// VirtIO Block Request record
/*
struct virtio_block_request
{
    uint32_t Type;                  // 0: Read; 1: Write; 4: Flush; 11: Discard; 13: Write zeroes
    uint32_t Reserved;
    uint64_t Sector;
    uint8_t  Data[];                // Data's size must be a multiple of 512
    uint8_t  Status;                // 0: OK; 1: Error; 2: Unsupported
};*/

struct virtq_desc
{

};

/*
struct VirtualQueue
{
    struct Buffers[QueueSize]
    {
        uint64_t Address;   // 64-bit address of the buffer on the guest machine.
        uint32_t Length;    // 32-bit length of the buffer.
        uint16_t Flags;     // 1: Next field contains linked buffer index;  2: Buffer is write-only (clear for read-only).
                            // 4: Buffer contains additional buffer addresses.
        uint16_t Next;      // If flag is set, contains index of next buffer in chain.
  }
 
    struct Available
    {
        uint16_t Flags;             // 1: Do not trigger interrupts.
        uint16_t Index;             // Index of the next ring index to be used.  (Last available ring buffer index+1)
        uint16_t [QueueSize] Ring;  // List of available buffer indexes from the Buffers array above.
        uint16_t EventIndex;        // Only used if VIRTIO_F_EVENT_IDX was negotiated
    }
 
    uint8_t[] Padding;  // Reserved
    // 4096 byte alignment
    struct Used
    {
        uint16_t Flags;            // 1: Do not notify device when buffers are added to available ring.
        uint16_t Index;            // Index of the next ring index to be used.  (Last used ring buffer index+1)
        struct Ring[QueueSize]
        {
            uint32_t Index;  // Index of the used buffer in the Buffers array above.
            uint32_t Length; // Total bytes written to buffer.
        }
        uint16_t AvailEvent;       // Only used if VIRTIO_F_EVENT_IDX was negotiated
    }
}
*/

#define VIRTQ_DESC_F_AVAIL              (1 << 7)
#define VIRTQ_DESC_F_USED               (1 << 15)
// This marks a descriptor as device write-only (otherwise device read-only)
#define VIRTQ_DESC_F_WRITE              2
// This marks a buffer as continuing
#define VIRTQ_DESC_F_NEXT               1
// This means the element contains a table of descriptors
#define VIRTQ_DESC_F_INDIRECT           4

// I/O mapped registers                 Offset
// Common registers
#define VIRTIO_IO_REG_DEVICE_FEATURES   0x00
#define VIRTIO_IO_REG_GUEST_FEATURES    0x04
#define VIRTIO_IO_REG_QUEUE_ADDRESS     0x08
#define VIRTIO_IO_REG_QUEUE_SIZE        0x0c
#define VIRTIO_IO_REG_QUEUE_SELECT      0x0e
#define VIRTIO_IO_REG_QUEUE_NOTIFY      0x10
#define VIRTIO_IO_REG_DEVICE_STATUS     0x12
#define VIRTIO_IO_REG_ISR STATUS        0x13

// Network Device Registers
#define VIRTIO_NET_IO_REG_MAC1          0x14
#define VIRTIO_NET_IO_REG_MAC2          0x15
#define VIRTIO_NET_IO_REG_MAC3          0x16
#define VIRTIO_NET_IO_REG_MAC4          0x17
#define VIRTIO_NET_IO_REG_MAC5          0x18
#define VIRTIO_NET_IO_REG_MAC6          0x19
#define VIRTIO_NET_IO_REG_STATUS        0x1a

// Block Device Registers
#define VIRTIO_BLK_IO_REG_TOTAL_SCT_CNT 0x14
#define VIRTIO_BLK_IO_REG_MAX_SEG_SIZE  0x1c
#define VIRTIO_BLK_IO_REG_MAX_SEG_CNT   0x20
#define VIRTIO_BLK_IO_REG_CYL_CNT       0x24
#define VIRTIO_BLK_IO_REG_HDR_CNT       0x26
#define VIRTIO_BLK_IO_REG_SCT_CNT       0x27
#define VIRTIO_BLK_IO_REG_BLK_LEN       0x28

// Device Status field
#define VIRTIO_DEVICE_S_ACKNOWLEDGED    0x01
#define VIRTIO_DEVICE_S_DRIVER_LOADED   0x02
#define VIRTIO_DEVICE_S_DRIVER_OK       0x04
#define VIRTIO_DEVICE_S_FEATURES_OK     0x08
#define VIRTIO_DEVICE_S_DEVICE_ERROR    0x40
#define VIRTIO_DEVICE_S_DRIVER_FAILED   0x80

// Feature Bits
// Network Device Controller
#define VIRTIO_NET_F_CSUM                   (1 << 0)
#define VIRTIO_NET_F_GUEST_CSUM             (1 << 1)
#define VIRTIO_NET_F_CTRL_GUEST_OFFLOADS    (1 << 2)
#define VIRTIO_NET_F_MTU                    (1 << 3)
#define VIRTIO_NET_F_MAC                    (1 << 5)
#define VIRTIO_NET_F_GUEST_TSO4             (1 << 7)
#define VIRTIO_NET_F_GUEST_TSO6             (1 << 8)
#define VIRTIO_NET_F_GUEST_ECN              (1 << 9)
#define VIRTIO_NET_F_GUEST_UFO              (1 << 10)
#define VIRTIO_NET_F_HOST_TSO4              (1 << 11)
#define VIRTIO_NET_F_HOST_TSO6              (1 << 12)
#define VIRTIO_NET_F_HOST_ECN               (1 << 13)
#define VIRTIO_NET_F_HOST_UFO               (1 << 14)
#define VIRTIO_NET_F_MRG_RXBUF              (1 << 15)
#define VIRTIO_NET_F_STATUS                 (1 << 16)
#define VIRTIO_NET_F_CTRL_VQ                (1 << 17)
#define VIRTIO_NET_F_CTRL_RX                (1 << 18)
#define VIRTIO_NET_F_CTRL_VLAN              (1 << 19)
#define VIRTIO_NET_F_GUEST_ANNOUNCE         (1 << 21)
#define VIRTIO_NET_F_MQ                     (1 << 22)
#define VIRTIO_NET_F_CTRL_MAC_ADDR          (1 << 23)
#define VIRTIO_NET_F_RSC_EXT                (1 << 61)
#define VIRTIO_NET_F_STANDBY                (1 << 62)

void virtio_net_init(uint8_t bus, uint8_t device, uint8_t function);

#endif
