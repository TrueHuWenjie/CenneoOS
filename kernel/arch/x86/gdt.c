// Cenneo OS
// /kernel/arch/x86/gdt.c
// Global Descriptor Table

#include "include/x86types.h"
#include "include/gdt.h"

// Note that the first one descriptor can not be use
#define GDT_NUM 8191 + 1
X86U64 gdt[GDT_NUM];

void gdt_delete(GDT_SEL selector)
{

}

GDT_SEL gdt_create(X86Addr base, X86U32 length, X86U32 attribute)
{

}

void init_gdt()
{
    
}
