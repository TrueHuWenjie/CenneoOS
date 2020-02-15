#include <stdlib.h>
#include <mmu.h>

#define VMALLOC_PHYLINER 1
#define VMALLOC_PREALLOC 2
#define VMALLOC_MAPONLY 4
#define VMALLOC_SWAPALLOW 8

/**内核区域大小*/
#define RANG_KERNEL_SIZE		268435456
#if (RANG_KERNEL_SIZE % 4194304)
	#error The size of range of kernel can not be devided by 4194304!
#endif

/**内核内存字节页图*/
#define KER_MEM_BYTEMAP_PTR		0x100000
#define KER_MEM_BYTEMAP_SIZE	RANG_KERNEL_SIZE / 4096
unsigned char *ker_mem_bytemap = (unsigned char *) KER_MEM_BYTEMAP_PTR;

/**允许分配回收的内核区域起始位置*/
#define KERNEL_ALLOC_START		KER_MEM_BYTEMAP_PTR + KER_MEM_BYTEMAP_SIZE

/**内核内存字节页图中每项的值的含义*/
#define KER_MEM_BYTEMAP_FREE	0b00000000			// 空闲页
#define KER_MEM_BYTEMAP_USED	0b00000001			// 已经使用的内存块
#define KER_MEM_BYTEMAP_START	0b00000010			// 已经使用的内存块的起始页
#define KER_MEM_BYTEMAP_END		0b00000100			// 已经使用的内存块的末尾页

/**设置内核内存字节页图函数*/
void set_ker_bytemap(unsigned long ptr, char flag)
{
	ker_mem_bytemap[ptr >> 12] = flag;
}


void *vmalloc(unsigned long size, unsigned char attribute)
{
    // Check attribute
    if (!size) return NULL;
    if (attribute && VMALLOC_MAPONLY)
        if (VMALLOC_PHYLINER + VMALLOC_PREALLOC + VMALLOC_SWAPALLOW)
            return NULL;

	unsigned long n = (KERNEL_ALLOC_START >> 12), l;

	/**对参数化整*/
	if ((size & 0xfff) != 0) size += 0x1000;
	size = size & 0xfffff000;

/**n为正在判断的空闲块的首页，l为正在判断的页*/
	/**寻找足够长的内存区域*/
	for (;;)
	{
		/**寻找到空闲区域*/
		if (ker_mem_bytemap[n] == KER_MEM_BYTEMAP_FREE)
		{
			/**循环检查该空闲块长度*/
			for (l = n;;l ++)
			{
				/**如果这个空闲块到这里为止且不够大小*/
				if (ker_mem_bytemap[l] != KER_MEM_BYTEMAP_FREE)
				{
					/**返回并且从下一个页开始找*/
					n = l + 1;
					break;
				}

				/**如果长度符合要求*/
				if ((l - n + 1) == (size >> 12))
				{
					/**长度合适跳转到分配模块*/
					goto allocate;
				}
			}

		}

		/**寻找下一个内存块*/
		n ++;

		/**判断寻找是否超过内核内存区域*/
		if (n >= KER_MEM_BYTEMAP_SIZE)
		{
			/**允许调度*/
			enable_schedule();
			/**超过了则分配失败*/
			return NULL;
		}

		/**判断剩余内核内存块长度是否满足要求*/
		if ((KER_MEM_BYTEMAP_SIZE - n) < (size >> 12))
		{
			/**允许调度*/
			enable_schedule();
			return NULL;
		}
	}

/**分配模块，该模块将相关内存块设置为已经使用*/
/**n为可用的自由内存块首地址，l为偏移*/
allocate:
	/**首先对首位进行特别标记*/
	ker_mem_bytemap[n] = KER_MEM_BYTEMAP_START | KER_MEM_BYTEMAP_USED;
	ker_mem_bytemap[n + (size >> 12) - 1] = ker_mem_bytemap[n + (size >> 12) - 1] | KER_MEM_BYTEMAP_END | KER_MEM_BYTEMAP_USED;

	/**将相关内存块都标记为占用*/
	for (l = n; (l - n + 1) != (size >> 12); l ++)
	{
		ker_mem_bytemap[l] = ker_mem_bytemap[l] | KER_MEM_BYTEMAP_USED;
	}

	/**允许调度*/
	enable_schedule();

	/**正常返回*/
	return (void *) (n << 12);
}

/**大块内存释放函数*/
int vfree(void *addr)
{
	/**判断该地址是否为这个被分配的内存块的首地址*/
	if ((ker_mem_bytemap[(unsigned long) addr >> 12] & KER_MEM_BYTEMAP_START) != KER_MEM_BYTEMAP_START)
	{
		/**输出错误信息*/
		printk("free the memory is error: this address is not the head of the block\n");
	}
	unsigned long n;

	/**循环回收内存块*/
	for (n = ((unsigned long) addr >> 12); ((ker_mem_bytemap[n] & KER_MEM_BYTEMAP_END) != KER_MEM_BYTEMAP_END); n ++)
	{
		/**回收内存块*/
		ker_mem_bytemap[n] = KER_MEM_BYTEMAP_FREE;
	}

	/**回收末尾的内存块*/
	ker_mem_bytemap[n] = KER_MEM_BYTEMAP_FREE;

	/**正常退出*/
	return 0;
}

/**页目录表*/
unsigned long *pdt, *pt;

/**创建新页目录函数*/
unsigned long new_pdt(void)
{
	unsigned long ptr;
	unsigned long *new_pdt;

	/**分配内存创建新的页目录表*/
	for (new_pdt = NULL; new_pdt == NULL; )
		new_pdt = vmalloc(MMU_PAGE_SIZE, 0);

	/**将已经出来的页目录表的所有页表都拷贝到新页目录表中*/
	for (ptr = 0; ptr < (RANG_KERNEL_SIZE / 4194304); ptr ++)
	{
		new_pdt[ptr] = pdt[ptr];
	}
}

/**虚拟空间映射函数*/
unsigned int kmap(unsigned int vir_addr, unsigned int phy_addr, unsigned int size)
{
	/**如果虚拟地址和物理地址非4KB对齐，则错误返回*/
	if ((vir_addr & 0xfff) != 0) return 1;
	if ((phy_addr & 0xfff) != 0) return 2;

	/**如果映射长度非4KB对齐，也错误返回*/
	if ((size & 0xfff) != 0) return 3;

	for (;size != 0;size -= MMU_PAGE_SIZE)
	{
		pt[(vir_addr >> 12)] = phy_addr + 0x7;
		phy_addr += MMU_PAGE_SIZE;
		vir_addr += MMU_PAGE_SIZE;
	}

	/**正常返回*/
	return 0;
}
