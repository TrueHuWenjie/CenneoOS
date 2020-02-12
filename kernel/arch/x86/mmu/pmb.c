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

/**设置页使用函数*/
void pmb_setused(unsigned long ptr)
{
	pmb[ptr >> 17] |= (1 << ((ptr >> 12) & 0b11111));
}

/**设置页自由函数*/
void pmb_setfree(unsigned long ptr)
{
	pmb[ptr >> 17] &= ~(1 << ((ptr >> 12) & 0b11111));
}

/**获取一个物理页函数
 * 返回值：NULL代表获取空闲物理页失败，非NULL代表获取成功。
 */
X86Addr pmb_alloc(void)
{
	unsigned long n, n2, new_page;
	unsigned int bitmap;

	/**先判断phy_mem_bitmap数组中是否有个32位元素不为0xffffffff*/
	for (n = 0; n < MMD_PMB_SIZE / sizeof(unsigned int); n ++)
	{
		if (pmb[n] != 0xffffffff)
		{
			/**如果某个元素中表示的页集中有空闲的页，则从中寻找*/
			bitmap = pmb[n];

			/**循环看这个页集中哪个页是空闲的*/
			for (n2 = 0; n2 < 32; n2 ++)
			{
				/**进行判断相应描述的位是否为0*/
				if (((bitmap >> n2) & 1) == 0)
				{
					/**这个页的实际地址计算出来*/
					new_page = (n * 32 * MMU_PAGE_SIZE) + (n2 * MMU_PAGE_SIZE);

					/**设置这个页为占用*/
					pmb_setused(new_page);

					/**返回这个页*/
					printk("    New page:%#X.\n", new_page);
					return (void *)new_page;
				}
			}
		}
	}

	/**当运行到这里的时候，代表遍历了页位图却无合适的页，暂时只能返回NULL值，以后可以在这里实现页交换*/

	/**无效返回*/
	return X86A_NULL;
}

/**释放一个物理页函数*/
void pmb_free(void *addr)
{
	/**将该页设置为自由*/
	pmb_setfree((unsigned long)addr);
	printk("    Free page:%#X.\n", addr);
}

// Initialization for Physical Memory Bitmap
void init_pmb(void)
{
    X86U32 n;
    X86U32 BaseAddr, Length;

    // Assuming all physical memory been used first
	for (n = 0; n < (MMD_PMB_SIZE / sizeof(X86U32)); n ++)
		pmb[n] = 0xffffffff;

    // Generating available memory bitmap by analysing EBI
    for (n = 0; n < BOOT_ARDS_NUM; n ++)
    {
        /**总内存计数*/
        pmb_sm.total += ebi.ARDS[n].LengthLow / MMU_PAGE_SIZE;

        /**判断是否是高于4GB的范围*/
        if (ebi.ARDS[n].BaseAddrHigh != 0) break;

        /**4KB对齐*/
    	ebi.ARDS[n].BaseAddrLow &= 0xfffff000;
    	ebi.ARDS[n].LengthLow &= 0xfffff000;

        /**判断该ARDS是否可用*/
        if (ebi.ARDS[n].Type != ARDS_FREE) continue;

        /**判断该ARDS的范围是否为0*/
        if (ebi.ARDS[n].LengthLow == 0) continue;

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
            pmb_setfree(BaseAddr);

            /**计数增加*/
            BaseAddr += MMU_PAGE_SIZE;
            Length -= MMU_PAGE_SIZE;
        }
    }

    printk("Installed memory(RAM):%dMB(%dKB is available).\n", \
        PMB_TOTAL_BYTES / 1048576, PMB_FREE_BYTES / 1024);
}
