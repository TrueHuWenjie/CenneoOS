/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Interrupt Management
 * Explorer/arch/x86/interrupt/interrupt.c
 * version:Alpha
 * 12/22/2014 2:52 PM
 */

#include "interrupt.h"
#include "../include/x86mmd.h"
#include "../include/function.h"
#include <stddef.h>
#include <types.h>

struct irqaction irqaction[NUMBER_INTERRUPT];

void idt_clean(void)
{
	unsigned int n, *idt;

	idt = (unsigned int *)MMD_IDT_ADDR;
	for (n = 0; n < MMD_IDT_SIZE / sizeof(unsigned int); n ++)
		idt[n] = 0;

	/**初始化irq数组*/
	unsigned long point;
	for (point = 0; point < NUMBER_INTERRUPT; point ++)
	{
		irqaction[point].name = NULL;
		irqaction[point].handle = &easy_handle;
	}
}

void init_interrupt(void)
{
	/**初始化中断描述符表*/
	x86_idtr_write(MMD_IDT_ADDR, MMD_IDT_SIZE - 1);

	idt_clean();

	/**创建中断描述符*/
	idt_create(0 , code_0_selector, &int_0 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(1 , code_0_selector, &int_1 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(2 , code_0_selector, &int_2 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(3 , code_0_selector, &int_3 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(4 , code_0_selector, &int_4 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(5 , code_0_selector, &int_5 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(6 , code_0_selector, &int_6 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(7 , code_0_selector, &int_7 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(8 , code_0_selector, &int_8 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(9 , code_0_selector, &int_9 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(10, code_0_selector, &int_10, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(11, code_0_selector, &int_11, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(12, code_0_selector, &int_12, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(13, code_0_selector, &int_13, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(14, code_0_selector, &int_14, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(15, code_0_selector, &int_15, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(16, code_0_selector, &int_16, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(17, code_0_selector, &int_17, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(18, code_0_selector, &int_18, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(19, code_0_selector, &int_19, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(20, code_0_selector, &int_20, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(21, code_0_selector, &int_21, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(22, code_0_selector, &int_22, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(23, code_0_selector, &int_23, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(24, code_0_selector, &int_24, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(25, code_0_selector, &int_25, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(26, code_0_selector, &int_26, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(27, code_0_selector, &int_27, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(28, code_0_selector, &int_28, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(29, code_0_selector, &int_29, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(30, code_0_selector, &int_30, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(31, code_0_selector, &int_31, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(32, code_0_selector, &int_32, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(33, code_0_selector, &int_33, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(34, code_0_selector, &int_34, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(35, code_0_selector, &int_35, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(36, code_0_selector, &int_36, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(37, code_0_selector, &int_37, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(38, code_0_selector, &int_38, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(39, code_0_selector, &int_39, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(40, code_0_selector, &int_40, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(41, code_0_selector, &int_41, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(42, code_0_selector, &int_42, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(43, code_0_selector, &int_43, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(44, code_0_selector, &int_44, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(45, code_0_selector, &int_45, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(46, code_0_selector, &int_46, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(47, code_0_selector, &int_47, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(48, code_0_selector, &int_48, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	idt_create(49, code_0_selector, &int_49, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);

	/**专门用作系统调用的描述符*/
	idt_create(50, code_0_selector, &int_50, interrupt_gate + IDT_32 + IDT_DPL_3 + IDT_P);


	/**初始化Intel 386保护模式的相关中断异常处理程序*/
	init_trap();

	return;
}


/**注册中断请求线函数
 * irq为中断号
 * name为对这个中断的描述
 * handle为处理这个中断的指针
 */
long register_irq(unsigned char irq, char *name, void (*handle)(int error_code))
{
	irqaction[irq].name = name;
	irqaction[irq].handle = handle;
	return 0;
}


/**初始化Intel 386保护模式的相关中断异常函数*/
void init_trap(void)
{
	register_irq(0, "Divide Error", &excp_de_handle);
	register_irq(1, "Debug", &excp_db_handle);
	// Non-maskable external interrupt.
	register_irq(3, "Breakpoint", &excp_bp_handle);
	register_irq(4, "Overflow", &excp_of_handle);
	register_irq(5, "BOUND Range Exceeded", &excp_br_handle);
	register_irq(6, "Invalid Opcode", &excp_ud_handle);
	register_irq(7, "Double Fault", &excp_nm_handle);
	register_irq(8, "Double Fault", &excp_df_handle);
	// Coprocessor Segment Overrun (reserved)
	register_irq(10, "Invalid TSS", &excp_ts_handle);
	register_irq(11, "Segment Not Present", &excp_np_handle);
	register_irq(12, "Stack Segment Fault", &excp_ss_handle);
	register_irq(13, "General Protection", &excp_gp_handle);
	// Page Fault
	// Intel reserved. Do not use.
	register_irq(16, "Floating-Point Error", &excp_mf_handle);
	register_irq(17, "Alignment Check", &excp_ac_handle);
	register_irq(18, "Machine Check", &excp_mc_handle);
	register_irq(19, "SIMD Floating-Point Exception", &excp_xf_handle);
}
