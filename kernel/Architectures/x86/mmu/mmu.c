// Cenneo OS
// /kernel/arch/x86/mmu/mmu.c
// Memory Management Unit

#include <stddef.h>
#include <stdlib.h>
#include <lib/string.h>
#include <page.h>
#include "../include/x86mmd.h"
#include "../include/x86ebi.h"
#include "../include/mmu.h"

/**初始化内存管理单元函数*/
void init_mmu(void)
{
	unsigned long n;

	init_pmb();

	/**少于MMD_MINI_MEM的情况不能下一步初始化*/
	if (PMB_TOTAL_BYTES < MMD_MINI_MEM) poweroff();

	/**进入分页模式*/
	init_paging();

	return;
}
