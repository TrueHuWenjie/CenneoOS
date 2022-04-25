/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer 页故障处理程序
 * Explorer/arch/x86/kernel/do_page_fault.c
 * version:Alpha
 * 8/7/2014 10:46 AM
 */

#include "../include/x86types.h"
#include "../include/x86mmd.h"
#include "../include/mmu.h"
#include <lib/string.h>
#include <stdlib.h>

#define PF_NOPG	0
#define PF_LEVL	1

static int rentry = 0;

/**页故障处理函数*/
void do_page_fault(int error_code)
{
	X86U32 cr2, *pd, *pt, *new_page, *pt2pt;

	/**读取CR2信息*/
	cr2 = x86_cr2_read();

	/**判断是否重入*/
	if (rentry != 0) error("Rentry happend in function 'do_page_fault()', \
		in addr:%#x.", cr2);
	else rentry = 1;

	pd = (X86U32 *)MMD_VM_CUR_PD_ADDR;
	pt = (X86U32 *)MMD_VM_CUR_PT_ADDR;
	pt2pt = MMD_VM_CUR_PT_ADDR + (MMD_VM_CUR_PT_ADDR >> 22) * MMU_PAGE_SIZE;

	/**判断是否是缺页引发的中断*/
	if ((error_code & 1) == PF_NOPG)
	{
		new_page = pmb_alloc(1);
		if (!new_page) error("No enough memory!");
		paging_map(new_page, (X86Addr *)cr2, 1, PAGING_MAP_RING0ONLY | PAGING_MAP_WRITABLE);
		goto finish;
	}else error("Page fault:(Unknown)error code:0x%X", error_code);

finish:
	rentry = 0;
	return;
}
