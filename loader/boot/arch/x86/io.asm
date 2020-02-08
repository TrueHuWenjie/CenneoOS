; Loader for Cenneo OS
; /boot/arch/x86/io.asm
; Input and output functions on x86 architecture

; Globally visible
global	io_in8,  io_in16,  io_in32		; Input instructions
global	io_out8, io_out16, io_out32		; Output instructions

; Functions
io_hlt:
	hlt
	jmp		io_hlt
io_in8:
	mov		edx,[esp+4]
	xor		eax,eax
	in		al,dx
	ret
io_in16:
	mov		edx,[esp+4]
	xor		eax,eax
	in		ax,dx
	ret
io_in32:
	mov		edx,[esp+4]
	in		eax,dx
	ret
io_out8:
	mov		edx,[esp+4]
	mov		al,[esp+8]
	out		dx,al
	ret
io_out16:
	mov		edx,[esp+4]
	mov		eax,[esp+8]
	out		dx,ax
	ret
io_out32:
	mov		edx,[esp+4]
	mov		eax,[esp+8]
	out		dx,eax
	ret
