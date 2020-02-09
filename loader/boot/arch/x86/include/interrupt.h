// Loader for Cenneo OS
// /boot/arch/x86/include/interrupt.h
// Interrupt

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "types.h"

// Located at /boot/arch/x86/interrupt/entry.asm
extern X86Addr int_0x20, int_0x21, int_0x22, int_0x23;
extern X86Addr int_0x24, int_0x25, int_0x26, int_0x27;
extern X86Addr int_0x28, int_0x29, int_0x2A, int_0x2B;
extern X86Addr int_0x2C, int_0x2D, int_0x2E, int_0x2F;

// Located at /boot/arch/x86/interrupt/trap.asm
extern void IDTR_write(X86Addr base, unsigned short size);

void create_ID
  (X86U32 number, X86U32 selector, X86Addr offset, X86U32 attribute);

// 中断描述符表属性
#define IDT_P       0x8000
#define IDT_DPL_0   0x00
#define IDT_DPL_1   0x2000
#define IDT_DPL_2   0x4000
#define IDT_DPL_3   0x6000
#define IDT_16      0x00
#define IDT_32      0x800
#define IDT_TRAP    0x700
#define IDT_ITRP    0x600

#endif
