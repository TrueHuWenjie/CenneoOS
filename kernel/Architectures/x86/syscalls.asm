; Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
; made by Lab Explorer Developers<1@GhostBirdOS.org>
; Explorer x86 syscalls support
; Explorer/arch/x86/syscalls.asm
; 2/8/2015 10:23 PM

; Registers
; EAX = Function Number
; EBX = Arguments

global do_syscalls				; Process Syscalls
extern sys_urm
extern sys_ipc					; MSG API函数
extern sys_gui_api				; GUI API函数

%define NUM_SYS_FUNCTION 32		; 最多功能数量

[section .data]

; 功能调用指针列表
sys_function_list:
	dd		sys_urm			; 0 Uniform Resource Manager
	dd		0				; 1 Memory Management
	dd		0				; 2 Process & Thread
	dd		sys_ipc			; 3 Interprocess Communication
	dd		0				; 4 File System
	dd		0				; 5 Network
	dd		sys_gui_api		; 6 GUI
.end	times (NUM_SYS_FUNCTION * 4) - (.end - sys_function_list) dd 0

[section .text]
[bits 32]
do_syscalls:
	; 判断需要的功能是否超过了功能数量的限制
	cmp		eax, (NUM_SYS_FUNCTION - 1)
	ja		.end

	; 根据EAX从系统调用列表中选择一个功能进行调用	
	; 重要：在这里EAX从功能号变成功能指令指针
	mov		eax, [((eax * 4) + sys_function_list)]
	
	push	edx
	push	ecx
	push	ebx
	push	eax

	; 判断该功能的指令指针是否为NULL
	cmp		eax, 0x0
	jz		.finish

	push	dword .finish			; 压入返回指针
	push	eax
	ret
	
; 完成系统调用
.finish:
	add		esp, 16
	
; 结束系统调用
.end:
	popad							; 还原上下文
	add		esp, 8					; 恢复栈
	popfd
	iretd							; 中断返回
	