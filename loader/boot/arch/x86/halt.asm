; Loader for Cenneo OS
; /boot/arch/x86/halt.asm
; Halt function on x86 architecture

; Globally visible
global halt					; Just hlt instruction
global idle					; halt forever

; Functions
[bits 32]
[section .text]
; void halt(void);
halt:
	hlt
	ret

; void idle(void);
idle:
	hlt
	jmp	idle

