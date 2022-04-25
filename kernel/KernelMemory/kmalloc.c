// Cenneo OS
// /kernel/kmm/kmalloc.c
// Kernel memory allocate & free

#include <stdlib.h>
#include <stddef.h>
#include <kvi.h>
#include <kmm.h>
#include "kmalloc.h"

#define MMU_PAGE_SIZE	4096

#define KMD_PER_PAGE	4096 / sizeof(struct kmd)


/**维护内存池
 * 注意：内存池中允许最大内存不得超过4096
 */
#define POOL_SIZE	8
static struct kmp kmp[POOL_SIZE] =
{
	{16  , 0, NULL},
	{32  , 0, NULL},
	{64  , 0, NULL},
	{128 , 0, NULL},
	{256 , 0, NULL},
	{512 , 0, NULL},
	{1024, 0, NULL},
	{2048, 0, NULL}
};

/**空闲内存描述符表的队列*/
static struct kmd *empty = NULL;

/**准备更多空闲内存描述符表函数*/
static void kmd_prepare(void)
{
	/**分配一个新页储存描述符*/
	struct kmd *MD = vmalloc(MMU_PAGE_SIZE, 0);
	if (MD == NULL) error("fill pool error!");

	/**初始化该页*/
	unsigned i;
	for (i = 0; i < KMD_PER_PAGE; i ++)
	{
		MD[i].next = &MD[i + 1];
	}

	/**将该页描述符加入到内存池中*/
	MD[i - 1].next = empty;
	empty = MD;
}

/**填充内存池函数*/
void fill_pool(unsigned long n)
{
	/**判断空闲内存描述符表是否空*/
	if (empty == NULL) kmd_prepare();

	/**获得一个页*/
	void *new_page = vmalloc(MMU_PAGE_SIZE, 0);

	/**判断是否获取成功*/
	if (new_page == NULL) error("kmm: No enough memory!");

	/**获取一个空闲内存描述符表*/
	struct kmd *new_MD = empty;
	empty = empty->next;

	/**获取相关信息*/
	size_t size = kmp[n].size;
	unsigned long number = MMU_PAGE_SIZE / size;

	/**初始化内存描述符表和这个页*/
	new_MD->page = new_page;
	new_MD->freeptr = new_page;
	new_MD->refcnt = 0;

	/**将内存描述符表加入到内存池中*/
	new_MD->next = kmp[n].next;
	kmp[n].next = new_MD;
	kmp[n].number = number;
}

/**内核小块内存分配函数
 * 参数:size - 请求的内存块长度
 * 返回值:NULL - 分配失败; !NULL - 分配成功，返回内存首地址
 */
void *kmalloc(size_t size, int flags)
{
	void *retval;

	/**不允许调度*/
	disable_schedule();

	/**内存描述符指针*/
	struct kmd *point;

	/**寻找合适大小的内存池*/
	unsigned long n;
	for (n = 0; n < POOL_SIZE; n ++)
	{
		if (kmp[n].size >= size)
		{
			/**执行到这里说明已经找到合适大小的内存池*/

			/**判断内存池中是否有足够的内存*/
			if (kmp[n].number == 0) fill_pool(n);

			/**获取可用内存*/
			retval = kmp[n].next->freeptr;
			kmp[n].next->refcnt ++;
			kmp[n].next->freeptr += kmp[n].size;
			kmp[n].number --;
			goto finish;
		}
	}
	/**运行到这里说明没有找到合适大小的内存池，
	 * 具体原因可能是参数size大于现有的最大内存池
	 * 可以分配的内存，这样的分配应当直接使用以页
	 * 为单位分配、回收的函数。
	 */
	//if (flags )
	error("argument is too long!");
	return NULL;

finish:
	/**已经完成了内存分配的处理*/
	/**允许调度*/
	enable_schedule();
	return retval;
}


/**内核小块内存释放函数
 * 参数:point - 指向要释放的内存首地址(之前kmalloc函数的返回值)
 */
void kfree(void *point)
{
	void *page;
	unsigned long n;
	struct kmd *MD, *prev = NULL;			/**指向当前描述符和上一个描述符*/

	/**计算获得该块内存所在的页面*/
	page = (void *)((unsigned long) point & 0xfffff000);

	/**寻找每个大小的内存池*/
	for (n = 0; n < POOL_SIZE; n ++)
	{
		/**寻找每个内存描述符*/
		for (MD = kmp[n].next; MD != NULL; MD = MD->next)
		{
			if (MD->page == page)
			{
				/**就是这个内存描述符*/
				MD->refcnt --;

				/**如果这个内存描述符引用为0*/
				if (MD->refcnt == 0)
				{
					if (prev != NULL)
					{
					}
				}
			}
			prev = MD;
		}
	}
}
