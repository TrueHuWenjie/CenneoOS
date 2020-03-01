// Cenneo OS
// /kernel/arch/x86/include/x86mmd.h
// Memory Model Design on x86 architecture

#ifndef X86MEM_H_
#define X86MEM_H_

#include "x86ebi.h"

// Unit defination
#define KB  * 1024
#define MB  * 1048576

// Minimum memory
#define MMD_MINI_MEM    8 MB

// #Notice: Hundreds of test show that the memory between 0x0~0x9fc00 are free
#define MMD_DATA_ADDR   0x00000000

// Design about Physical Memory Bitmap
#define MMD_PMB_ADDR    MMD_DATA_ADDR
#define MMD_PMB_SIZE    128 KB

// Design about Kernel Memory Bitmap
#define MMD_KMB_ADDR    0x00020000
#define MMD_KMB_SIZE    64 KB

// Design about Global Descriptor Table
#define MMD_GDT_ADDR    0x00030000
#define MMD_GDT_SIZE    64 KB

// Design about Interrupt Descriptor Table
#define MMD_IDT_ADDR    0x00040000
#define MMD_IDT_SIZE    2 KB

#define MMD_DATA_SIZE   ((MMD_IDT_ADDR + MMD_IDT_SIZE) + 2 KB)

#define MMD_KNEL_ADDR   ebi.kernel_addr
#define MMD_KNEL_SIZE   ebi.kernel_size

// Virtual Memory
#define MMD_VM_KNEL_ADDR     0x00000000
#define MMD_VM_KNEL_SIZE     (256 MB - 4096 * (1 + 1024))
#define MMD_VM_PD_ADDR  0xfbff000
#define MMD_VM_PD_SIZE  4096
#define MMD_VM_PT_ADDR  0xfc00000
#define MMD_VM_PT_SIZE  4096 * 1024

#endif
