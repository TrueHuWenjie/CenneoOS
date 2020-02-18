// Cenneo OS
// /kernel/arch/x86/mmu/paging.c
// Paging

#include "../include/x86types.h"
#include "../include/x86ebi.h"
#include "../include/x86mmd.h"
#include "../include/mmu.h"
#include "../include/kvi.h"
#include <lib/mem.h>

#define PG_PD_INDEX(addr) (addr >> 22)
#define PG_PT_INDEX(addr) (addr >> 12) & 0x3ff

pg_pd_ensure()
{

}

pg_pt_ensure()
{

}

/*pd_(pd, addr, size)
{
	// First makesure that the page table exist here

	addr
}
*/
/**
 * 进入分页模式函数
 * 分页模式是保护模式下一种可选的模式，对内存进行分页管理
 * 实现分页模式可以
 */
void goto_paging(X86Addr pd)
{
	/**将页目录表的地址写入CR3寄存器*/
	write_CR3(pd);

	/**设置CR0的最高位，开启分页模式*/
	write_CR0(read_CR0() | 0x80000000);
}

/**初始化分页模式函数*/
void init_paging(void)
{
    X86U32 *pd, *pt, *pt2pt;
    X86U32 ptr;

    // Build the Page Directory and some Page Table for accessing themselves
    pd = pmb_alloc();
	memset(pd, 0, MMU_PAGE_SIZE);

    pd[PG_PD_INDEX(MMD_VM_PD_ADDR)] = (X86U32)pmb_alloc() + 3;
	pt = (X86U32 *)(pd[PG_PD_INDEX(MMD_VM_PD_ADDR)] & 0xfffff000);
	memset(pt, 0, MMU_PAGE_SIZE);
	pt[PG_PT_INDEX(MMD_VM_PD_ADDR)] = (X86U32)pd + 3;

	pt2pt = pmb_alloc();
	memset(pt2pt, 0, MMU_PAGE_SIZE);
    pd[PG_PD_INDEX(MMD_VM_PT_ADDR)] = (X86U32)pt2pt + 3;
    pt2pt[PG_PD_INDEX(MMD_VM_PT_ADDR)] = pd[PG_PD_INDEX(MMD_VM_PT_ADDR)];

    // Maping the kernel and data area into virtual memory
    for (ptr = 0; ptr * 1024 < ebi.kernel_size; ptr += MMU_PAGE_SIZE)
	{
		pt2pt[PG_PD_INDEX(ebi.kernel_addr + ptr)] = pmb_alloc() + 3;
		pd[PG_PD_INDEX(ebi.kernel_addr + ptr)] = \
		pt2pt[PG_PD_INDEX(ebi.kernel_addr + ptr)];
		memset((X86Addr)pd[PG_PD_INDEX(ebi.kernel_addr + ptr)], \
		0, MMU_PAGE_SIZE);
	}

    for (ptr = 0; ptr < ebi.kernel_size; ptr += MMU_PAGE_SIZE)
	{
		pt = (X86U32 *)(pd[PG_PD_INDEX(ebi.kernel_addr + ptr)] & 0xfffff000);
        pt[PG_PT_INDEX(ebi.kernel_addr + ptr)] = ebi.kernel_addr + ptr | 3;
	}

    for (ptr = 0; ptr * 1024 < MMD_DATA_SIZE; ptr += MMU_PAGE_SIZE)
	{
		pt2pt[PG_PD_INDEX(MMD_DATA_ADDR + ptr)] = pmb_alloc() + 3;
        pd[PG_PD_INDEX(MMD_DATA_ADDR + ptr)] = \
		pt2pt[PG_PD_INDEX(MMD_DATA_ADDR + ptr)];
		memset((X86Addr)pd[PG_PD_INDEX(MMD_DATA_ADDR + ptr)], \
		0, MMU_PAGE_SIZE);
	}

    for (ptr = 0; ptr < MMD_DATA_SIZE; ptr += MMU_PAGE_SIZE)
	{
		pt = (X86U32 *)(pd[PG_PD_INDEX(MMD_DATA_ADDR + ptr)] & 0xfffff000);
		pt[PG_PT_INDEX(MMD_DATA_ADDR + ptr)] = MMD_DATA_ADDR + ptr | 3;
	}

	// Maping the graphics memory
	for (ptr = 0; ptr * 1024 < 3145728; ptr += MMU_PAGE_SIZE)
	{
		pd[PG_PD_INDEX(ebi.ModeInfoBlock.PhysBasePtr + ptr)] = \
		(X86U32)pmb_alloc() | 3;
		memset((X86Addr)pd[PG_PD_INDEX(ebi.ModeInfoBlock.PhysBasePtr + ptr)], \
		0, MMU_PAGE_SIZE);
	}


    for (ptr = 0; ptr < 3145728; ptr += MMU_PAGE_SIZE)
	{
		pt = (X86U32 *)(pd[PG_PD_INDEX(ebi.ModeInfoBlock.PhysBasePtr + ptr)] \
		& 0xfffff000);
        pt[PG_PT_INDEX(ebi.ModeInfoBlock.PhysBasePtr + ptr)] = \
		ebi.ModeInfoBlock.PhysBasePtr + ptr | 3;
	}

	register_irq(14, "Page Fault", &do_page_fault);

    goto_paging(pd);
}
