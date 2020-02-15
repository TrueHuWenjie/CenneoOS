// Cenneo OS
// /kernel/arch/x86/mmu/paging.c
// Paging

#include "../include/x86types.h"
#include "../include/x86ebi.h"
#include "../include/x86mmd.h"
#include "../include/mmu.h"
#include "../include/kvi.h"

#define PG_PD_INDEX(addr) (addr >> 22)
#define PG_PT_INDEX(addr) (addr >> 12) & 0x3ff

pg_pd_ensure()
{

}

pg_pt_ensure()
{

}

pg_()
{

}

/**
 * 进入分页模式函数
 * 分页模式是保护模式下一种可选的模式，对内存进行分页管理
 * 实现分页模式可以
 */
void goto_paging(X86Addr pdt)
{
	/**将页目录表的地址写入CR3寄存器*/
	write_CR3(pdt);

	/**设置CR0的最高位，开启分页模式*/
	write_CR0(read_CR0() | 0x80000000);
}


/**初始化分页模式函数*/
void init_paging(void)
{
    X86U32 ***pdt;
    unsigned int ptr;

    // Build the Page Directory and some Page Table for accessing themselves
    pdt = pmb_alloc();
    pdt[PG_PD_INDEX(MMD_VM_PD_ADDR)] = pmb_alloc();
    pdt[PG_PD_INDEX(MMD_VM_PT_ADDR)] = pmb_alloc();
    pdt[PG_PD_INDEX(MMD_VM_PD_ADDR)][PG_PT_INDEX(MMD_VM_PD_ADDR)] = pdt;

    pdt[PG_PD_INDEX(MMD_VM_PT_ADDR)][PG_PT_INDEX(MMD_VM_PT_ADDR)] = \
    pdt[PG_PD_INDEX(MMD_VM_PT_ADDR)];

    // Maping the kernel and data area into virtual memory
    for (ptr = 0; ptr * MMU_PAGE_SIZE * 1024 < ebi.kernel_size; ptr ++)
        pdt[PG_PD_INDEX(ebi.kernel_addr + ptr)] = pmb_alloc();

    for (ptr = 0; ptr * MMU_PAGE_SIZE < ebi.kernel_size; ptr ++)
        pdt[PG_PD_INDEX(ebi.kernel_addr + ptr)] \
        [PG_PT_INDEX(ebi.kernel_addr + ptr)] = ebi.kernel_addr + ptr;

    for (ptr = 0; ptr * MMU_PAGE_SIZE * 1024 < MMD_DATA_SIZE; ptr ++)
        pdt[PG_PD_INDEX(MMD_DATA_ADDR + ptr)] = pmb_alloc();

    for (ptr = 0; ptr * MMU_PAGE_SIZE < MMD_DATA_SIZE; ptr ++)
        pdt[PG_PD_INDEX(MMD_DATA_ADDR + ptr)] \
        [PG_PT_INDEX(MMD_DATA_ADDR + ptr)] = MMD_DATA_ADDR + ptr;

    goto_paging(pdt);













	//unsigned long ptr;

	/**分配页目录表*/
	//for (pdt = NULL; !pdt; )
	//	pdt = vmalloc(MMU_PAGE_SIZE);
	//printk("pdt = %#x", pdt);
	/**分配页表*/
	//for (pt = NULL; !pt; )
	//	pt = vmalloc((RANG_KERNEL_SIZE / MMU_PAGE_SIZE) * sizeof(pt));

	/**将所有页表都注册到页目录表*/
	//for (ptr = 0; ptr < (RANG_KERNEL_SIZE / 4194304); ptr ++)
	//{
	//	pdt[ptr] = (ptr * MMU_PAGE_SIZE) + (int)pt + 0x7;
	//}

	/**将所有内存区域的页都注册到页表中*/
	//for (ptr = 0; ptr < RANG_KERNEL_SIZE / 4096; ptr ++)
	//{
	//	pt[ptr] = ptr * MMU_PAGE_SIZE + 0x7;
	//}

	/**进入分页模式*/
	//goto_paging(pdt);

	/**打信息*/
	printk("Finished - init_paging();\n");
	fin:io_hlt(); goto fin;
}
