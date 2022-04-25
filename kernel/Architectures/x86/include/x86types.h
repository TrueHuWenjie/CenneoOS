// Loader for Cenneo OS
// /kernel/arch/x86/include/x86types.h
// Type definations on Intel x86-platform

#ifndef X86TYPES_H_
#define X86TYPES_H_

typedef void *X86Addr;
typedef unsigned char       X86U8;
typedef unsigned short      X86U16;
typedef unsigned int        X86U32;
typedef unsigned long long  X86U64;
typedef char                X86S8;
typedef short               X86S16;
typedef long long           X86S32;

#define X86A_NULL (void *) 0

#endif
