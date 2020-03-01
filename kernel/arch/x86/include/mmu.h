// Cenneo OS
// /kernel/arch/x86/include/mmu.h
// Memory Management Unit

#ifndef MMU_H_
#define MMU_H_

#include "x86types.h"

#define MMU_PAGE_SIZE	4096

#define MMU_SIZE_ALIGN(addr) { \
  \
}

struct pmb_sm
{
    X86U32 free;                    // In page
    X86U32 total;                   // In page
    X86U32 rsvd;                    // In page
};
extern struct pmb_sm pmb_sm;

#define PMB_FREE_BYTES  pmb_sm.free  * MMU_PAGE_SIZE
#define PMB_TOTAL_BYTES pmb_sm.total * MMU_PAGE_SIZE
#define PMB_RSVD_BYTES pmb_sm.rsvd * MMU_PAGE_SIZE

// Return the number of free physical page
unsigned long pmb_info_free(void);

// Lock functions about pmb, cannot be used by any other process/thread
void pmb_lock(void);

// Unlock functions about pmb
void pmb_unlock(void);

// Allocate a free page, return with non-X86A_NULL, otherwise failed.
X86Addr pmb_alloc(void);

/**释放一个物理页函数*/
void pmb_free(X86Addr addr);

// Memory maping
void paging_map(X86Addr phy_addr, X86Addr vir_addr, X86U32 number);

// Initialization for Physical Memory Bitmap
void init_pmb(void);

/**初始化分页模式函数*/
void init_paging(void);

/**页故障处理程序*/
void do_page_fault(int error_code);

#endif
