; Loader for Cenneo OS
; /boot/arch/x86/interrupt/trap.asm
; Interruption Management on x86 architecture

; Globally visible
global interrupt_close
global interrupt_start
global IDTR_write
global IDTR.base

; Functions
[bits 32]
[section .text]
; void interrupt_close(void);
interrupt_close:
	cli
	ret

; void interrupt_start(void);
interrupt_start:
	sti
	ret

; void write_IDT(unsigned int base, unsigned short size)
IDTR_write:
	mov	eax, [esp + 4]
	mov	[IDTR.base], eax
	mov	ax, [esp + 8]
	mov	[IDTR.size], ax
	lidt	[cs:IDTR]
	ret

; Data
[section .data]
; IDTR's structure
IDTR:
	.size	dw	0			; Size of IDT
	.base	dd	0			; Base address of IDT
