/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer 页故障处理程序
 * Explorer/arch/x86/kernel/do_page_fault.c
 * version:Alpha
 * 8/7/2014 10:46 AM
 */

#include <lib/mem.h>
#include "../include/x86types.h"
#include "../include/x86mmd.h"
#include "../include/mmu.h"
#include <stdlib.h>

int rentry = 0;

#define PF_NOPG	0
#define PF_LEVL	1

/**页故障处理函数*/
void do_page_fault(int error_code)
{
	X86U32 cr2, *pd, *pt, *new_page, *pt2pt;

	/**读取CR2信息*/
	cr2 = read_CR2();

	/**判断是否重入*/
	if (rentry != 0) error("Rentry happend in function 'do_page_fault()', \
		in addr:%#x.", cr2);
	else rentry = 1;

	pd = (X86U32 *)MMD_VM_PD_ADDR;
	pt = (X86U32 *)MMD_VM_PT_ADDR;
	pt2pt = MMD_VM_PT_ADDR + (MMD_VM_PT_ADDR >> 22) * MMU_PAGE_SIZE;

	/**判断是否是缺页引发的中断*/
	if ((error_code & 1) == PF_NOPG)
	{
		/**如果故障原因是没有页表*/
		if ((pd[cr2 >> 22] & 1) == 0)
		{
			/**获取一个新页*/
			new_page = pmb_alloc();

			if (!new_page) error("No enough memory!");

			/**放置新页作为页表（页目录表和页表均在内核态空间中）*/
			pd[cr2 >> 22] = ((unsigned int)new_page | 0x3);
			pt2pt[cr2 >> 22] = pd[cr2 >> 22];

			/**清空这个页，保证不会出现干扰*/
			memset(&pt[cr2 >> 22], 0x00, MMU_PAGE_SIZE);
		}

		//pt = (X86U32 *)(pd[(cr2 >> 22)] & 0xfffff000);

		/**无论是否出现缺页表的情况，一定缺少页*/
		new_page = pmb_alloc();
		pt[cr2 >> 12] = ((X86U32)new_page | 0x3);
		if (!new_page) error("No enough memory!");

		goto finish;
	}else error("Page fault:(Unknown)error code:0x%X", error_code);

finish:
	rentry = 0;
	return;
}
