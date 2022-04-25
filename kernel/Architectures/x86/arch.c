// Cenneo OS
// /kernel/arch/x86/arch.c
// HAL layer's initialization 'init_arch'

#include "include/x86types.h"
#include "include/x86ebi.h"
#include "include/x86mmd.h"
#include "include/mmu.h"
#include "include/function.h"
#include <mpt/mpt.h>
#include <types.h>
#include <lib/string.h>

struct boot_info ebi;

/**代码段、数据段、TSS段的选择子*/
X86U16 code_0_selector, data_0_selector, TSS_selector;
X86U16 code_3_selector, data_3_selector;

struct TSS_32 TSS;

/**架构初始化*/
void init_arch(void)
{
	// EBI check & store
	memcpy(&ebi, loader_ebi_ptr, sizeof(ebi));
	X86EBI_CHECK

	/**准备GDT表*/
	x86_gdtr_write(MMD_GDT_ADDR, MMD_GDT_SIZE - 1);
	memset(MMD_GDT_ADDR, 0, MMD_GDT_SIZE);

	/**创建数据段和代码段*/
	code_0_selector = gdt_set(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_0 + GDT_code_32_non_conforming);
	code_3_selector = gdt_set(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_3 + GDT_code_32_conforming);
	data_0_selector = gdt_set(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_0 + GDT_data_32);
	data_3_selector = gdt_set(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_3 + GDT_data_32);
	init_seg_reg(data_0_selector);

	/**准备TSS(真心不该有TSS):-(*/
	TSS_selector = gdt_set(((u32)&TSS), sizeof(TSS) - 1, GDT_P + GDT_DPL_0 + GDT_TSS_data);

	/**TSS初始赋值*/
	TSS.ss0 = data_0_selector;
	// TSS.esp0 = (unsigned long)&task_0 + TASK_SIZE;
	// TSS.ss1 = data_0_selector;
	// TSS.esp1 = 0;
	// TSS.ss2 = 0;
	// TSS.esp2 = 0;
	TSS.eip = 0;
	TSS.eflags = 0;
	TSS.eax = 0;
	TSS.ecx = 0;

	TSS.iomap = sizeof(TSS) - 2;
	TSS.ioend = 0xff;

	/**加载TSS*/
	x86_tr_write(TSS_selector);

	init_interrupt();
	init_mmu();
}
