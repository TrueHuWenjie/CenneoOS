; Explorer Boot
; /boot/boota.asm
; ASM file of the Explorer Boot


; 起始及初始化函数
global	init_IDT

extern IDTR.base

; 中断入口函数
extern	keyboard_handle
extern	i8259A_auto_return
extern	int_auto_return

global	int_auto
global	int_0x20
global	int_0x21
global	int_0x22
global	int_0x23
global	int_0x24
global	int_0x25
global	int_0x26
global	int_0x27
global	int_0x28
global	int_0x29
global	int_0x2A
global	int_0x2B
global	int_0x2C
global	int_0x2D
global	int_0x2E
global	int_0x2F

; 描述符寄存器操作函数
global	write_IDTR

; IDT操作函数
global	create_ID



[bits 32]
; Code section

[section .text]

	
; 创建一个中断描述符函数*/
create_ID:
;void create_ID(u32 number, u32 selector, u32 offset, u32 attribute)
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
	
; 中断入口函数
int_auto:
	call	int_auto_return
	iret
int_0x20:
	call	i8259A_auto_return
	iret
int_0x21:
	call	keyboard_handle
	iret
int_0x22:
	call	i8259A_auto_return
	iret
int_0x23:
	call	i8259A_auto_return
	iret
int_0x24:
	call	i8259A_auto_return
	iret
int_0x25:
	call	i8259A_auto_return
	iret
int_0x26:
	call	i8259A_auto_return
	iret
int_0x27:
	call	i8259A_auto_return
	iret
int_0x28:
	call	i8259A_auto_return
	iret
int_0x29:
	call	i8259A_auto_return
	iret
int_0x2A:
	call	i8259A_auto_return
	iret
int_0x2B:
	call	i8259A_auto_return
	iret
int_0x2C:
	call	i8259A_auto_return
	iret
int_0x2D:
	call	i8259A_auto_return
	iret
int_0x2E:
	call	i8259A_auto_return
	iret
int_0x2F:
	call	i8259A_auto_return
	iret

