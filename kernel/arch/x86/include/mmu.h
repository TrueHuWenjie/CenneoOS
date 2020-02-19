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

/**获取一个物理页函数
 * 返回值：NULL代表获取空闲物理页失败，非NULL代表获取成功。
 */
// Return the number of free physical page
unsigned long pmb_info_free(void);

X86Addr pmb_alloc(void);

/**释放一个物理页函数*/
void pmb_free(X86Addr addr);

// Initialization for Physical Memory Bitmap
void init_pmb(void);

/**初始化分页模式函数*/
void init_paging(void);

/**页故障处理程序*/
void do_page_fault(int error_code);

#endif
