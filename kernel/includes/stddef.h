/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer C standard definition
 * ExplorerOS/Kernel/include/stddef.h
 * version:Alpha
 * 8/7/2014 9:36 AM
 */

#ifndef STDDEF_H_
#define STDDEF_H_

/*
这里的__built_expect()函数是gcc(version >= 2.96)的内建函数,提供给程序员使用的，目的是将"分支转移"的信息提供给编译器，这样编译器对代码进行优化，以减少指令跳转带来的性能下降。
__buildin_expect((x), 1)表示x的值为真的可能性更大.
__buildin_expect((x), 0)表示x的值为假的可能性更大. 
*/
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define NULL  (void*)0

typedef unsigned int flags_t;
typedef unsigned long size_t;
typedef unsigned int address_t;
typedef unsigned int dma_addr_t;
typedef unsigned long ptrdiff_t;

#endif
