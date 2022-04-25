
extern init_process_environment
extern main
global _start
global syscall

[section .text]
[bits 32]
_start:
    call    init_process_environment
    call    main
    ret

syscall:
    mov     eax, [esp + 4]
    mov     ebx, [esp + 8]
    mov     ecx, [esp + 12]
    mov     edx, [esp + 16]
    int     50
    ret

[section .data]
