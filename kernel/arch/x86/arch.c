// Cenneo OS
// /kernel/arch/x86/arch.c
// HAL layer's initialization 'init_arch'

#include "include/x86types.h"
#include "include/x86ebi.h"
#include "include/x86mmd.h"
#include "include/kvi.h"
#include "include/function.h"
#include <task.h>
#include <types.h>

/**代码段、数据段、TSS段的选择子*/
X86U16 code_0_selector, data_0_selector, TSS_selector;
X86U16 code_3_selector, data_3_selector;

struct TSS_32 TSS;


/**内存管理相关信息输出函数*/
void output_mem_info(void)
{
	unsigned long n;

	/**空出一行*/
	printk("\n");

	/**打印内存分布信息*/
	printk("Address Range Descriptor Structure:\nBaseAddrLow   BaseAddrHigh  LengthLow     LengthHigh    Type\n");
	for (n = 0; n < BOOT_ARDS_NUM; n++)
	{
		printk("%#010x    %#010x    %#010x    %#010x    %#010x\n", \
		ebi.ARDS[n].BaseAddrLow, \
		ebi.ARDS[n].BaseAddrHigh, ebi.ARDS[n].LengthLow, \
		ebi.ARDS[n].LengthHigh, ebi.ARDS[n].Type);
	}
}

/**架构初始化*/
void init_arch(void)
{
	// EBI check & store
	X86EBI_CHECK
	x86ebi_store();

	// Temporary
	init_graph();
	init_font();

	// Open kvi
	kvi_open();

	output_mem_info();

	/**准备GDT表*/
	write_GDTR(MMD_GDT_ADDR, MMD_GDT_SIZE - 1);
	// clean_GDT();

	/**创建数据段和代码段*/
	code_0_selector = set_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_0 + GDT_code_32_non_conforming);
	code_3_selector = set_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_3 + GDT_code_32_conforming);
	data_0_selector = set_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_0 + GDT_data_32);
	data_3_selector = set_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_3 + GDT_data_32);
	init_seg_reg(data_0_selector);

	/**准备TSS(真心不该有TSS):-(*/
	TSS_selector = set_GDT(((u32)&TSS), sizeof(TSS) - 1, GDT_P + GDT_DPL_0 + GDT_TSS_data);

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
	write_TR(TSS_selector);

	printk("Finished - init();\n");
}

/**
 * 进入分页模式函数
 * 分页模式是保护模式下一种可选的模式，对内存进行分页管理
 * 实现分页模式可以
 */
void goto_paging(u32 pdt_addr)
{
	/**将页目录表的地址写入CR3寄存器*/
	write_CR3(pdt_addr);

	/**设置CR0的最高位，开启分页模式*/
	write_CR0(read_CR0() | 0x80000000);
}

/**重置函数*/
void reset(void)
{
	io_out8(0x64,0xfe);
}
