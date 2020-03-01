// Cenneo OS
// /kernel/arch/x86/include/gdt.c
// Global Descriptor Table

#ifndef GDT_H_
#define GDT_H_

#include "x86types.h"

typedef X86U16 GDT_SEL;

void gdt_delete(GDT_SEL selector);

GDT_SEL gdt_create(X86Addr base, X86U32 length, X86U32 attribute);

#endif
