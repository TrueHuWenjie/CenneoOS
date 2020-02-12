// Cenneo OS
// /kernel/arch/x86/include/x86mmd.h
// Memory Model Design on x86 architecture

#ifndef X86MEM_H_
#define X86MEM_H_

// Unit defination
#define KB  * 1024
#define MB	* 1048576

// #Notice: Hundreds of test show that the memory between 0x0~0x9fc00 are free

// Design about Physical Memory Bitmap
#define MMD_PMB_ADDR    0x00000000
#define MMD_PMB_SIZE    128 KB

// Design about Kernel Memory Bitmap
#define MMD_KMB_ADDR    0x00020000
#define MMD_KMB_SIZE    64 KB

// Design about Global Descriptor Table
#define MMD_GDT_ADDR    0x00030000
#define MMD_GDT_SIZE    64 KB

#endif
