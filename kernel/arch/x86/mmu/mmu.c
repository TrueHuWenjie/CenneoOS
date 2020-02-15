// Cenneo OS
// /kernel/arch/x86/mmu/mmu.c
// Memory Management Unit

#include <memory.h>
#include <stddef.h>
#include <stdlib.h>
#include <lib/mem.h>
#include <../include/page.h>
#include "../include/x86mmd.h"
#include "../include/x86ebi.h"
#include "../include/kvi.h"
#include "../include/mmu.h"

/**初始化内存管理单元函数*/
void init_mmu(void)
{
	unsigned long n;

	init_pmb();

	/**少于MMD_MINI_MEM的情况不能下一步初始化*/
	if (PMB_TOTAL_BYTES < MMD_MINI_MEM)
		error("Not enough memory!\n" \
			"Please make sure the memory more than the kernel required.\n");

	/**进入分页模式*/
	init_paging();
fin:goto fin;
	/**正常返回*/
	printk("Finished - init_mmu();\n");
	return;
}
