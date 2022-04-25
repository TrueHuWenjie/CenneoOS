// Cenneo OS
// /kernel/kmm/vmap.c
// Implementation of memory mapping technology

#include <stddef.h>
#include <mmu.h>
#include "kmm_sm.h"

// Map physical memory into virtual memory
int vmap(void *phy_addr, void *vir_addr, size_t size)
{
    unsigned long n;
    
    // Check attribute
    if (!size) return -1;
    for (n = 0; n < kmm_sm.ksdt_len; n ++)
        if ((kmm_sm.ksdt[n].attr & KSD_ATTR_USED) && \
            (kmm_sm.ksdt[n].attr & KSD_ATTR_MAPD) && \
            ((kmm_sm.ksdt[n].addr << 12) == (unsigned long)vir_addr) && \
            ((kmm_sm.ksdt[n].size << 12) >= size))
            goto map;

    return -2;

map:
    paging_map(phy_addr, vir_addr, size >> 12, PAGING_MAP_RING0ONLY | PAGING_MAP_WRITABLE);

	/**正常返回*/
	return 0;
}
