// Cenneo OS
// /kernel/kmm/vmalloc.c
// Vast memory allocate & free

#include <stdlib.h>
#include <kvi.h>
#include <mmu.h>
#include <kmm.h>
#include "kmm_sm.h"

// Vast memory allocate functions in kernel area
void *vmalloc(size_t size, unsigned char attribute)
{
	unsigned long n, target;

    // Check attribute
    if (!size) return NULL;
    if (attribute & VM_MAPD)
	{
		if ((attribute & VM_CTND) ||
			(attribute & VM_IMMD) ||
			(attribute & VM_SWAP))
            return NULL;
	}else if (attribute & VM_CTND)
	{
		if (attribute & VM_IMMD)
			return NULL;
	}

	// Align the paraments 'size'
	if (size % MMU_PAGE_SIZE)
		size = MMU_PAGE_SIZE + (size & 0xfffff000);

	// First we need to know whether there are enough free space
	if (kmm_sm.free < size)
	{
		if (attribute & VM_WAIT)
			error("vmalloc is unable to meet demand but still waiting here!");

		// We may add some solutions here in future
		return NULL;
	}

ksdt_seek:
	// Seeking for a continuous space
	for (n = 0; n < kmm_sm.ksdt_len; n ++)
	{
		if (kmm_sm.ksdt[n].attr != KSD_ATTR_FREE) continue;
		if ((kmm_sm.ksdt[n].size << 12) < size) continue;
		goto page_seek;
	}

	// In here we have no found any continuous space
	if (attribute & VM_WAIT)
		error("There is no continuous memory segment in kernel state!");

	return NULL;

page_seek:
	// Only the call with attribute 'VM_IMMD' need do this
	if (!(attribute & VM_IMMD)) goto alloc;

	// Lock pmb functions
	pmb_lock();

	// In here we need to check out free physical memory
	if ((pmb_info_free() << 12) < size)
	{
		error("vmalloc is unable to meet demand but still waiting here!");
		pmb_unlock();
		return NULL;
	}


	if (attribute && VM_CTND)
	{
		printk("Unsupported here: vmalloc with attribute 'VM_CTND'!\n");
		return NULL;
	}else{
		printk("Unsupported here: vmalloc with attribute 'VM_CTND'!\n");
		return NULL;
	}

alloc:
	target = kmm_sm.ksdt[n].addr << 12;
	kmm_sm.ksdt[kmm_sm.ksdt_len].addr = kmm_sm.ksdt[n].addr;
	kmm_sm.ksdt[kmm_sm.ksdt_len].size = (size >> 12);
	kmm_sm.ksdt[n].addr += (size >> 12);
	kmm_sm.ksdt[n].size -= (size >> 12);
	if (!kmm_sm.ksdt[n].size) kmm_sm.ksdt[n].attr = KSD_ATTR_FREE;

	if (attribute && VM_MAPD)
	{
		kmm_sm.ksdt[kmm_sm.ksdt_len].attr = KSD_ATTR_USED + KSD_ATTR_MAPD;
		kmm_sm.ksdt_len ++;
		kmm_sm.free -= size;
		kmm_sm.mapd += size;
		return (void *)target;
	}

error("vmalloc error!");
return NULL;
//kmm_sm.ksdt[n].size -= (size >> 12);
//((kmm_sm.ksdt[n].addr << 12) + size);

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

/**页目录表*/
unsigned long *pdt, *pt;

/**创建新页目录函数*/
unsigned long new_pdt(void)
{
	unsigned long ptr;
	unsigned long *new_pdt;

	/**分配内存创建新的页目录表*/
	for (new_pdt = NULL; new_pdt == NULL; )
		new_pdt = vmalloc(MMU_PAGE_SIZE, 0);

	/**将已经出来的页目录表的所有页表都拷贝到新页目录表中*/
	//for (ptr = 0; ptr < (RANG_KERNEL_SIZE / 4194304); ptr ++)
	//{
	//	new_pdt[ptr] = pdt[ptr];
	//}
}
