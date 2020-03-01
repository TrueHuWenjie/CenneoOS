; Explorer Boot
; /boot/boota.asm
; ASM file of the Explorer Boot

extern IDTR.base

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
