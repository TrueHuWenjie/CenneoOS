 // Cenneo OS
 // /kernel/arch/x86/interrupt/interrupt.h
 // Explorer Interrupt Management Definition

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#define NUMBER_INTERRUPT	256

/**中断响应表*/
struct irqaction
{
	void (*handle)(int errnum);
	char *name;
};

extern struct irqaction irqaction[NUMBER_INTERRUPT];

/**上下文(通用寄存器)被压入堆栈的顺序*/
struct context
{
	long edi;		/**esp + 4*/
	long esi;		/**esp + 8*/
	long ebp;		/**esp + 12*/
	long esp;		/**esp + 16*/
	long ebx;		/**esp + 20*/
	long edx;		/**esp + 24*/
	long ecx;		/**esp + 28*/
	long eax;		/**esp + 32*/
};


/**中断的汇编函数入口*/
void int_0(void);
void int_1(void);
void int_2(void);
void int_3(void);
void int_4(void);
void int_5(void);
void int_6(void);
void int_7(void);
void int_8(void);
void int_9(void);
void int_10(void);
void int_11(void);
void int_12(void);
void int_13(void);
void int_14(void);
void int_15(void);
void int_16(void);
void int_17(void);
void int_18(void);
void int_19(void);
void int_20(void);
void int_21(void);
void int_22(void);
void int_23(void);
void int_24(void);
void int_25(void);
void int_26(void);
void int_27(void);
void int_28(void);
void int_29(void);
void int_30(void);
void int_31(void);
void int_32(void);
void int_33(void);
void int_34(void);
void int_35(void);
void int_36(void);
void int_37(void);
void int_38(void);
void int_39(void);
void int_40(void);
void int_41(void);
void int_42(void);
void int_43(void);
void int_44(void);
void int_45(void);
void int_46(void);
void int_47(void);
void int_48(void);
void int_49(void);
void int_50(void);

/**interrupt.c中*/

/**注册中断请求线函数
 * irq为中断号
 * name为对这个中断的描述
 * handle为处理这个中断的指针
 */
long register_irq(unsigned char irq, char *name, void (*handle)(int error_code));

/**初始化Intel 386保护模式的相关中断异常函数*/
void init_trap(void);

// Divide Error
void excp_de_handle(int error_code);

// Debug
void excp_db_handle(int error_code);

// Breakpoint
void excp_bp_handle(int error_code);

// Overflow
void excp_of_handle(int error_code);

// BOUND Range Exceeded
void excp_br_handle(int error_code);

// Invalid Opcode (Undefined Opcode)
void excp_ud_handle(int error_code);

// Device Not Available (No Math Coprocessor)
void excp_nm_handle(int error_code);

// Double Fault
void excp_df_handle(int error_code);

// Invalid TSS
void excp_ts_handle(int error_code);

// Segment Not Present
void excp_np_handle(int error_code);

// Stack-Segment Fault
void excp_ss_handle(int error_code);

// General Protection
void excp_gp_handle(int error_code);

// x87 FPU Floating-Point Error (Math Fault)
void excp_mf_handle(int error_code);

// Alignment Check
void excp_ac_handle(int error_code);

// Machine Check
void excp_mc_handle(int error_code);

// SIMD Floating-Point Exception
void excp_xf_handle(int error_code);

/**handle.c中*/

/**中断处理函数*/
void interrupt_handle(struct context context, unsigned char irq, int error_code);

/**中断简单处理函数*/
void easy_handle(int error_code);

#endif
