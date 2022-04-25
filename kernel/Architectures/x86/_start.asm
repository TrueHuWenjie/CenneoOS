; Cenneo OS
; /kernel/arch/x86/_start.asm
; the entrypoint of kernel

;全局变量及全局函数
extern	task_0				;任务0的联合体，在Explorer/arch/x86/kernel/task/task.c中定义
extern	init_arch			;初始化架构函数，在Explorer/arch/x86/Architecture.c中实现
extern	main				;内核的主函数，位于Explorer/init/main.c中实现
global	_start				;内核的入口函数
global	loader_ebi_ptr				;指向boot_info的指针*/

;TSC函数
global	tsc_read
;初始化段寄存器函数
global	init_seg_reg
;描述符寄存器操作函数
global	x86_idtr_write
global	x86_tr_write
global	x86_gdtr_write
;IDT操作函数
global	idt_clean
global	idt_create
;GDT操作函数
global	gdt_set
;控制寄存器读写
global	x86_cr0_read,x86_cr0_write
global	x86_cr2_read,x86_cr2_write
global	x86_cr3_read,x86_cr3_write
;特殊大小内存读写函数
global	write_mem24

TASK_SIZE	equ	8192

;相关宏定义
;void x86_idtr_write(u32 IDTR.base, u16 IDTR.size)
%macro call_x86_idtr_write 2
	push	word %2
	push	dword %1
	call	x86_idtr_write
	add		esp,6
%endmacro
;void x86_gdtr_write(u32 GDTR.base, u16 GDTR.size)
%macro call_x86_gdtr_write 2
	push	word %2
	push	dword %1
	call	x86_gdtr_write
	add		esp,6
%endmacro
;void x86_tr_write(u16 selector)
%macro call_x86_tr_write 1
	push	word %1
	call	x86_tr_write
	add		esp,2
%endmacro
;u16 gdt_set(u32 segment_base, u32 limit, u32 attribute)
%macro call_gdt_set 3
	push	dword %3
	push	dword %2
	push	dword %1
	call	gdt_set
	add		esp,12
%endmacro
;void idt_create(u32 number, u32 selector, u32 offset, u32 attribute)
%macro call_idt_create 4
	push	dword %4
	push	dword %3
	push	dword %2
	push	dword %1
	call	idt_create
	add		esp,16
%endmacro

;全局描述符表的相关宏定义
%define GDT_G 0x800000
%define GDT_P 0x8000
%define GDT_DPL_0 0x00
%define GDT_DPL_1 0x2000
%define GDT_DPL_2 0x4000
%define GDT_DPL_3 0x6000
;注意:都为可读的代码段
%define GDT_code_32_conforming 0x401E00
%define GDT_code_32_non_conforming 0x401A00
;为向上的数据段，向下的数据段有风险，不可使用
%define GDT_data_32 0x200
;系统段
%define GDT_TSS_data 0x900



;NOTICE:能自由使用的寄存器只有EAX\ECX\EDX
[section .text]
[bits 32]

;整个内核的入口函数
_start:
	; Get loader_ebi_ptr
	mov		eax, [esp + 4]
	mov		[loader_ebi_ptr], eax

	; 初始化堆栈指针指向内核栈
	mov		esp,task_0 + TASK_SIZE

	; 初始化平台相关信息
	call	init_arch

	; 调用主函数开始进行各项的初始化
	call	main

	; 怠速运行
.sleep:
	hlt
	jmp		.sleep
	

;平台相关控制代码

;初始化段寄存器函数
init_seg_reg:
	mov		ax,[esp+4]
	mov		ds,ax
	mov		es,ax
	mov		fs,ax
	mov		gs,ax
	mov		ss,ax
	ret

;时间戳寄存器(Time Stamp Counter，TSC)从奔腾(Pentium)系列开始被引入，
;是x86 CPU中一个特殊的寄存器，该寄存器在CPU每经过一个时钟周期时加1，
;当计算机reset后，TSC将被清空。
;TSC是个64位的寄存器，使用rdtsc指令读取它。指令执行后，EAX：EDX存放TSC的值。
;3GHz的CPU运行195年，TSC寄存器才会溢出。

;TSC读取函数
tsc_read:
	rdtsc
	ret

;描述符寄存器操作函数
x86_idtr_write:
;void write_IDT(u32 base, u16 size)
	mov		eax,[esp+4]
	mov		[IDTR.base],eax
	mov		ax,[esp+8]
	mov		[IDTR.size],ax
	lidt	[cs:IDTR]		;加载IDTR
	ret
x86_tr_write:
;void x86_tr_write(u16 select)
	mov		ax,[esp+4]
	ltr		ax
	ret
x86_gdtr_write:
;void x86_gdtr_write(u32 base, u16 size)
	mov		eax,[esp+4]
	mov		[GDTR.base],eax
	mov		ax,[esp+8]
	mov		[GDTR.size],ax
	lgdt	[cs:GDTR]		;加载GDTR
	ret

;IDT操作函数
idt_create:
;void idt_create(u32 number, u32 selector, u32 offset, u32 attribute)
	xor		eax,eax
	mov		al,[esp+4]	;number参数
	shl		eax,3		;相当于乘8
	add		eax,[IDTR.base];加上addr的起始地址
	mov		dx,[esp+8]	;selector参数
	mov		[eax+2],dx
	mov		edx,[esp+12]	;offset参数
	mov		[eax],dx	;dx=offset低16位
	shr		edx,16
	mov		[eax+6],dx	;dx=offset高16位
	mov		word[eax+4],0;清空属性区域
	mov		edx,[esp+16];attribute参数
	add		[eax+4],edx	;加上属性
	ret

gdt_set:
;u16 gdt_set(u32 segment_base, u32 limit, u32 attribute)
	push	ebp
	mov		ebp,esp
	push	esi
	;表基地址
	mov		esi,[GDTR.base]
.loop:
	;跳过空描述符以及循环查找功能
	add		esi,8
	;判断该GDT表项是否是8字节的0
	cmp		dword[esi],0x00
	jnz		.loop
	cmp		dword[esi+4],0x00
	jnz		.loop
	;将段基址放置到GDT中
	mov		eax,[ebp+8]
	mov		[esi+2],ax
	shr		eax,16
	mov		[esi+4],al
	mov		[esi+7],ah
	;将界限放置到GDT
	mov		eax,[ebp+12]
	mov		[esi],ax
	;震荡eax,保证高12位为0
	shl		eax,12
	shr		eax,12+16
	mov		[esi+6],al
	;将属性加入表项中
	mov		eax,[ebp+16]
	add		[esi+4],eax
	;计算出select value
	mov		eax,esi
	sub		eax,[GDTR.base]
	pop		esi
	pop		ebp
	ret

;控制寄存器的读写
x86_cr0_read:
	mov		eax, cr0
	ret
x86_cr2_read:
	mov		eax, cr2
	ret
x86_cr3_read:
	mov		eax, cr3
	ret
x86_cr0_write:
	mov		eax, [esp+4]
	mov		cr0, eax
	ret
x86_cr2_write:
	mov		eax, [esp+4]
	mov		cr2, eax
	ret
x86_cr3_write:
	mov		eax, [esp+4]
	mov		cr3, eax
	ret



write_mem24:
	mov		edx, [esp+8]
	mov		ecx, [esp+4]
	mov		[ecx], dx
	shr		dx, 16
	mov		[ecx+2], dl
	ret

;数据区
[section .data]
;启动信息结构体指针*/
loader_ebi_ptr	dd	0



;GDTR
GDTR:
	.size		dw	0			;GDT的长度
	.base		dd	0			;GDT的物理地址
;IDTR
IDTR:
	.size		dw	0			;IDT的长度
	.base		dd	0			;IDT的物理地址
