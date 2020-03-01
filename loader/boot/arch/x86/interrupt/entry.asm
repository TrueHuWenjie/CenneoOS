; Loader for Cenneo OS
; /boot/arch/x86/interrupt/entry.asm
; Interruption's entry

; Globally visible
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

; Functions
[bits 32]
[section .text]

; The entry of Interruption
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
