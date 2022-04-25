// Cenneo OS
// /kernel/kmm/vmalloc.c
// Vast memory allocate & free

#include <stdlib.h>
#include <kvi.h>
#include <mmu.h>
#include <kmm.h>
#include "kmm_sm.h"

// Vast memory allocate functions in kernel area
void *vmalloc(size_t size, int flags)
{
	X86Addr phys_target;
	unsigned long n, m, virt_target;

    // Check flags
    if (!size) return NULL;
    if (flags & VM_MAPD)
	{
		if ((flags & VM_IMMD) ||
			(flags & VM_SWAP))
            return NULL;
	}else if (flags & VM_CTND)
		flags |= VM_IMMD;

	// Align the paraments 'size'
	if (size % MMU_PAGE_SIZE)
		size = MMU_PAGE_SIZE + (size & 0xfffff000);

ksdt_check:
	// First we need to know whether there are enough free space
	if (kmm_sm.free < size)
	{
		if (flags & VM_WAIT)
			error("vmalloc is unable to meet demand but still waiting here!");

		// We may add some solutions here in future
		return NULL;
	}

	// Seeking for a continuous kernel space
	for (n = 0; n < kmm_sm.ksdt_len; n ++)
	{
		if (kmm_sm.ksdt[n].attr != KSD_ATTR_FREE) continue;
		if ((kmm_sm.ksdt[n].size << 12) < size) continue;
		virt_target = kmm_sm.ksdt[n].addr << 12;
		goto page_check;
	}

	// In here we have no found any continuous space
	if (flags & VM_WAIT)
		error("There is no continuous memory segment in Kernel-Space!");
	return NULL;

// Ignore this step if flag VM_IMMD is not set
page_check:
	if (!(flags & VM_IMMD)) goto ksdt_alloc;

	// Lock pmb functions
	pmb_lock();

	// In here we need to check out free physical memory
	if ((pmb_info_free() << 12) < size)
	{
		error("vmalloc is unable to meet demand but still waiting here!");
		pmb_unlock();
		return NULL;
	}

page_alloc:
	
	if (flags && VM_CTND)
	{
		phys_target = (unsigned long)pmb_alloc(size >> 12);
		if (!phys_target)
		{
			pmb_unlock();

			if (flags & VM_WAIT)
				error("There is no continuous memory segment in Physical Memory!");
			
			return NULL;
		}
		
		paging_map(phys_target, (X86Addr)virt_target, size >> 12, PAGING_MAP_RING0ONLY | PAGING_MAP_WRITABLE);
	}else if (flags && VM_IMMD)
	{
		for (m = 0; m < size >> 12; m ++)
		{
			phys_target = pmb_alloc(1);
			paging_map(phys_target, (X86Addr)(virt_target + m << 12), 1, PAGING_MAP_RING0ONLY | PAGING_MAP_WRITABLE);
		}
	}

	pmb_unlock();

ksdt_alloc:
	kmm_sm.ksdt[kmm_sm.ksdt_len].addr = kmm_sm.ksdt[n].addr;
	kmm_sm.ksdt[kmm_sm.ksdt_len].size = (size >> 12);
	kmm_sm.ksdt[n].addr += (size >> 12);
	kmm_sm.ksdt[n].size -= (size >> 12);
	if (!kmm_sm.ksdt[n].size) kmm_sm.ksdt[n].attr = KSD_ATTR_FREE;

	if (flags && VM_MAPD)
	{
		kmm_sm.ksdt[kmm_sm.ksdt_len].attr = KSD_ATTR_USED + KSD_ATTR_MAPD;
		kmm_sm.mapd += size;
	}else{
		kmm_sm.ksdt[kmm_sm.ksdt_len].attr = KSD_ATTR_USED;
		kmm_sm.used += size;
		
	}

	kmm_sm.free -= size;
	kmm_sm.ksdt_len ++;
	return (void *)virt_target;
}

// Vast memory free functions in kernel area
int vfree(void *addr)
{
	/**判断该地址是否为这个被分配的内存块的首地址*/
	//if ((ker_mem_bytemap[(unsigned long) addr >> 12] & KER_MEM_BYTEMAP_START) != KER_MEM_BYTEMAP_START)
	//{
		/**输出错误信息*/
	//	printk("free the memory is error: this address is not the head of the block\n");
	//}
	unsigned long n;

	/**循环回收内存块*/
	//for (n = ((unsigned long) addr >> 12); ((ker_mem_bytemap[n] & KER_MEM_BYTEMAP_END) != KER_MEM_BYTEMAP_END); n ++)
	//{
		/**回收内存块*/
		//ker_mem_bytemap[n] = KER_MEM_BYTEMAP_FREE;
	//}

	/**回收末尾的内存块*/
	//ker_mem_bytemap[n] = KER_MEM_BYTEMAP_FREE;

	/**正常退出*/
	return 0;
}
