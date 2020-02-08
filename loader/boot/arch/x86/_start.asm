; Loader for Cenneo OS
; /boot/arch/x86/_start.asm
; _start function on x86 architecture

; Globally visible
global _start
extern BOOT_main

; Functions
[bits 32]
[section .text]
_start:
	call	BOOT_main
