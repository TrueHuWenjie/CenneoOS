// Cenneo OS
// /kernel/kmm/kmalloc.h
// Kernel Memory Allocation

#ifndef KMALLOC_H_
#define KMALLOC_H_

// Kernel Memroy Pool
struct kmp
{
	size_t size;
	unsigned long number;
	struct kmd *next;
};

// Kernel Memory Descriptor
struct kmd
{
	void *page;
	void *freeptr;
	unsigned short refcnt;
	struct kmd *next;
};
/**其中flags代表下面情况之一*/
#define MEM_INVALID	0
#define MEM_FREE	1
#define MEM_USED	2


/**内核小块内存分配函数
 * 参数:size - 请求的内存块长度
 * 返回值:NULL - 分配失败; !NULL - 分配成功，返回内存首地址
 */
void *kmalloc(size_t size, int flags);

/**内核小块内存释放函数
 * 参数:point - 指向要释放的内存首地址(之前kmalloc函数的返回值)
 */
void kfree(void *point);

#endif
