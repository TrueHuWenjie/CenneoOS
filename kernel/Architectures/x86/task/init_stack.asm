;Copyright 2013-2015 by Explorer Developers.
;made by Lab Explorer Developers<1@GhostBirdOS.org>
;Explorer task Initialize stack
;Explorer/arch/x86/kernel/task/Init_stack.asm
;version:Alpha
;11/14/2014 9:12 PM

extern	temp_stack			; unsigned long temp_stack;/*临时堆栈指针存放变量，在\arch\x86\kernel\task\task.c中定义*/
extern	exit				; void exit_Task(void);/*销毁线程函数，在\arch\x86\kernel\task\task.c中定义*/
extern	code_0_selector		; u32 code_0_selector;/*0级代码段选择子，在\arch\x86\include\function.h中定义*/
extern	code_3_selector
extern	data_3_selector
global	task_stack_init		; extern unsigned long task_stack_init(unsigned long stack, int (*function)(void *), void * argument);
global	x86_rpl_switch
global	x86_cpl_switch

[section .text]
[bits 32]

task_save_retvalue:
	push	eax						; Save Return Value
	call	exit
	jmp		$

; 初始化内核任务堆栈
; dword[ESP+12]是新任务的参数指针
; dword[ESP+8]是任务的函数指针
; dword[ESP+4]是需要初始化的堆栈的栈顶指针
; 返回值eax是已经初始化过的堆栈的栈顶指针
task_stack_init:
	mov		ecx, [esp + 8]
	mov		edx, [esp + 12]
	mov		esi, esp				; 保存esp
	mov		esp, [esp + 4]			; 切换堆栈到需要初始化堆栈的栈顶
	push	edx						; 参数指针
	push	dword task_save_retvalue
	push	ecx						; 任务的函数指针
	push	dword 1000000000b		; 标志寄存器
	push	ebp
	mov		eax, esp				; 将已经初始化过的堆栈的栈顶指针作为返回值
	mov		esp, esi				; 还原堆栈
	ret

x86_cpl_switch:
	mov		ax, [data_3_selector]
	push	test_stack + 512
	push	dword 1000000000b
	mov		ax, [code_3_selector]
	push	ax
	push	dword [esp + 4]
	iretd


[section .data]
[bits 32]


test_stack	times 512 db 0 