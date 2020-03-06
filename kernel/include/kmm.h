/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Memory Management definition
 * Explorer/include/memory.h
 * 11/15/2014 12:05 PM
 */

 // Cenneo OS
 // /kernel/include/memory.h
 // Kernel Memory Management

#ifndef KMM_H_
#define KMM_H_

#include <stddef.h>
#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

// Attribute for function 'vmalloc'
#define VM_CTND 1		// Physically continuous
#define VM_IMMD 2		// Allocate all space immediately
#define VM_MAPD 4		// Only for map
#define VM_SWAP 8		// Swap allow
#define VM_WAIT 16		// Allow vmalloc waiting until success

// Vast memory allocate & free functions in kernel area
void *vmalloc(size_t size, int flags);

// Map physical memory into virtual memory
int vmap(void *phy_addr, void *vir_addr, size_t size);

// Vast memory free functions in kernel area
int vfree(void *addr);

// Flags for function 'kmalloc'
// #define KM_

void *kmalloc(size_t size, int flags);
void kfree(void *point);

/**创建新页目录函数*/
unsigned long new_pdt(void);

void kmm_info(void);

unsigned int kmm_info_free(void);

void init_kmm(void);

#ifdef __cplusplus
}
#endif

#endif
