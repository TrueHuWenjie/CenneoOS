// Cenneo OS
// /kernel/kmm/kmm.c
// Kernel Memory Management

#include "../arch/x86/include/x86mmd.h"
#include "../arch/x86/include/mmu.h"
#include "kmm_sm.h"
#include <kvi.h>

struct kmm_sm kmm_sm;

void kmm_info(void)
{
    unsigned long i;

    for (i = 0; i < kmm_sm.ksdt_len; i ++)
        printk("ksdt[%d]: addr = %#010x, size = %#010x, attr = %d.\n", i, \
        kmm_sm.ksdt[i].addr << 12, kmm_sm.ksdt[i].size << 12, \
        kmm_sm.ksdt[i].attr);

    printk("kmm: free:%dKB, used:%dKB, mapd:%dKB.\n", kmm_sm.free / 1024, \
    kmm_sm.used / 1024, kmm_sm.mapd / 1024);
}

unsigned int kmm_info_free(void)
{
    return kmm_sm.free;
}

void init_kmm(void)
{
    unsigned long i, size;

    // Build the Kernel Segment Descriptor Table now
    kmm_sm.ksdt = (struct ksd *)KSDT_ADDR;

    kmm_sm.ksdt[0].addr = MMD_DATA_ADDR >> 12;
    kmm_sm.ksdt[0].size = MMD_DATA_SIZE >> 12;
    kmm_sm.ksdt[0].attr = KSD_ATTR_USED + KSD_ATTR_MAPD + KSD_ATTR_CTND;

    kmm_sm.ksdt[2].addr = MMD_KNEL_ADDR >> 12;
    kmm_sm.ksdt[2].size = MMD_KNEL_SIZE >> 12;
    kmm_sm.ksdt[2].attr = KSD_ATTR_USED + KSD_ATTR_MAPD + KSD_ATTR_CTND;

    kmm_sm.ksdt[1].addr = kmm_sm.ksdt[0].addr + kmm_sm.ksdt[0].size;
    kmm_sm.ksdt[1].size = kmm_sm.ksdt[2].addr - kmm_sm.ksdt[1].addr;
    kmm_sm.ksdt[1].attr = KSD_ATTR_FREE;

    kmm_sm.ksdt[3].addr = kmm_sm.ksdt[2].addr + kmm_sm.ksdt[2].size;
    kmm_sm.ksdt[3].size = (KSDT_ADDR >> 12) - (kmm_sm.ksdt[2].addr + \
    kmm_sm.ksdt[2].size);
    kmm_sm.ksdt[3].attr = KSD_ATTR_FREE;

    kmm_sm.ksdt_len = 4;

    // Then calculate memory usage in kernel area
    for (i = 0; i < kmm_sm.ksdt_len; i ++)
    {
        size = kmm_sm.ksdt[i].size << 12;
        if (kmm_sm.ksdt[i].attr == KSD_ATTR_FREE)
            kmm_sm.free += size;
        else if (kmm_sm.ksdt[i].attr && KSD_ATTR_MAPD)
            kmm_sm.mapd += size;
        else kmm_sm.used += size;
    }
}
