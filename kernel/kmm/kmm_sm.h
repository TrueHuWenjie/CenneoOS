// Cenneo OS
// /kernel/kmm/kmm_sm.h
// Kernel Memory Management - State Machine

#ifndef KMM_SM_H_
#define KMM_SM_H_

// Kernel Segment Descriptor
struct ksd
{
    unsigned long addr:20;
    unsigned long size:20;
    unsigned long attr:8;
};

#define KSD_ATTR_FREE   0
#define KSD_ATTR_USED   1
#define KSD_ATTR_MAPD   2
#define KSD_ATTR_SWAP   4
#define KSD_ATTR_CTND   8


#define KSDT_SIZE       sizeof(struct ksd) * (MMD_VM_KNEL_SIZE / MMU_PAGE_SIZE)
#define KSDT_ADDR       ((MMD_VM_KNEL_ADDR + MMD_VM_KNEL_SIZE - KSDT_SIZE) & \
0xfffff000)

struct kmm_sm
{
    struct ksd *ksdt;
    unsigned long ksdt_len;
    unsigned long free;
    unsigned long used;
    unsigned long mapd;
};

// In kmm.c
extern struct kmm_sm kmm_sm;

#endif
