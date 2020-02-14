// Cenneo OS
// /kernel/arch/x86/mmu/pmb.c
// Physical Memory Bitmap

#include "../include/x86types.h"
#include "../include/x86mmd.h"
#include "../include/x86ebi.h"
#include "../include/kvi.h"
#include "../include/mmu.h"

X86U32 *pmb = (X86U32 *) MMD_PMB_ADDR;

struct pmb_sm pmb_sm;

#define PMB_PAGE_FREE 0
#define PMB_PAGE_USED 1

// Set page as used
#define PMB_SETUSED(addr) pmb[addr >> 17] |= (1 << ((addr >> 12) & 0b11111))

// Set page as free
#define PMB_SETFREE(addr) pmb[addr >> 17] &= ~(1 << ((addr >> 12) & 0b11111))

// Check if a page is free
#define PMB_IS_FREE(addr) \
	pmb[(X86U32)addr >> 17] >> (((X86U32)addr >> 12) & 0b11111) & 1

/**获取一个物理页函数
 * 返回值：NULL代表获取空闲物理页失败，非NULL代表获取成功。
 */
X86Addr pmb_alloc(void)
{
	unsigned long n, i, new_page;
	unsigned int bitmap;

	/**先判断phy_mem_bitmap数组中是否有个32位元素不为0xffffffff*/
	for (n = 0; n < MMD_PMB_SIZE / sizeof(unsigned int); n ++)
	{
		if (pmb[n] != 0xffffffff)
		{
			/**如果某个元素中表示的页集中有空闲的页，则从中寻找*/
			bitmap = pmb[n];

			/**循环看这个页集中哪个页是空闲的*/
			for (i = 0; i < 32; i ++)
			{
				/**进行判断相应描述的位是否为0*/
				if (((bitmap >> i) & 1) == PMB_PAGE_FREE)
				{
					/**这个页的实际地址计算出来*/
					new_page = (n * 32 * MMU_PAGE_SIZE) + (i * MMU_PAGE_SIZE);

					/**设置这个页为占用*/
					PMB_SETUSED(new_page);
					pmb_sm.free --;

					/**返回这个页*/
					printk("    New page:%#X.\n", new_page);
					return (X86Addr)new_page;
				}
			}
		}
	}

	/**当运行到这里的时候，代表遍历了页位图却无合适的页，暂时只能返回NULL值，以后可以在这里实现页交换*/

	/**无效返回*/
	return X86A_NULL;
}

/**释放一个物理页函数*/
void pmb_free(X86Addr addr)
{
	if (PMB_IS_FREE(addr) == PMB_PAGE_FREE) return;

	/**将该页设置为自由*/
	PMB_SETFREE((unsigned long)addr);
	pmb_sm.free ++;
	printk("    Free page:%#X.\n", addr);
}

// Initialization for Physical Memory Bitmap
void init_pmb(void)
{
    X86U32 n;
    X86U32 BaseAddr, Length;
	X86U32 kernel_addr, kernel_size;

    // Assuming all physical memory been used first
	for (n = 0; n < (MMD_PMB_SIZE / sizeof(X86U32)); n ++)
		pmb[n] = 0xffffffff;

    // Generating available memory bitmap by analysing EBI
    for (n = 0; n < BOOT_ARDS_NUM; n ++)
    {
        /**判断是否是高于4GB的范围*/
        if (ebi.ARDS[n].BaseAddrHigh != 0) break;

        /**4KB对齐*/
    	ebi.ARDS[n].BaseAddrLow &= 0xfffff000;
    	ebi.ARDS[n].LengthLow &= 0xfffff000;

		/**判断该ARDS的范围是否为0*/
		if (!ebi.ARDS[n].LengthLow) continue;

        /**判断该ARDS是否可用*/
        if (ebi.ARDS[n].Type != ARDS_FREE)
		{
			pmb_sm.rsvd += ebi.ARDS[n].LengthLow / MMU_PAGE_SIZE;
			continue;
		}else pmb_sm.total += ebi.ARDS[n].LengthLow / MMU_PAGE_SIZE;

        /**归纳信息*/
        BaseAddr = ebi.ARDS[n].BaseAddrLow;
        Length = ebi.ARDS[n].LengthLow;
    	pmb_sm.free += Length / MMU_PAGE_SIZE;

    	// Print info
    	printk("    Available memory:%#x~%#x.\n", BaseAddr, BaseAddr + Length);

        /**制作相应的物理内存位页图和内核内存字节页图*/
        while (Length)
    	{
            /**设置空闲*/
            PMB_SETFREE(BaseAddr);

            /**计数增加*/
            BaseAddr += MMU_PAGE_SIZE;
            Length -= MMU_PAGE_SIZE;
        }
    }

    printk("    Installed memory(RAM):%dKB, reserved memory(ROM):%dKB.\n", \
        PMB_TOTAL_BYTES / 1024, PMB_RSVD_BYTES / 1024);

	// Marking the place used by kernel and other system data as 'used'
	kernel_addr = ebi.kernel_addr & 0xfffff000;
	if (ebi.kernel_size % MMU_PAGE_SIZE)
		kernel_size = MMU_PAGE_SIZE + ebi.kernel_size & 0xfffff000;
	else kernel_size = ebi.kernel_size;

	printk("    Data's addr:%#x, size:%dKB.\n", \
		MMD_DATA_ADDR, MMD_DATA_SIZE / 1024);
	printk("    Kernel's addr:%#x, size:%d(%d)KB.\n", \
		kernel_addr, kernel_size / 1024, ebi.kernel_size);

	for (n = 0; n * MMU_PAGE_SIZE < MMD_DATA_SIZE; n ++)
		PMB_SETUSED(MMD_DATA_ADDR + n * MMU_PAGE_SIZE);

	for (n = 0; n * MMU_PAGE_SIZE < kernel_size; n ++)
		PMB_SETUSED(kernel_addr + n * MMU_PAGE_SIZE);

	pmb_sm.free -= (kernel_size + MMD_DATA_SIZE) / MMU_PAGE_SIZE;

	printk("    Free:%dPages(%dKB), Total:%dPages(%dKB).\n", \
	pmb_sm.free, PMB_FREE_BYTES / 1024, pmb_sm.total, PMB_TOTAL_BYTES / 1024);

	printk("Finished - init_pmb();\n");
}
