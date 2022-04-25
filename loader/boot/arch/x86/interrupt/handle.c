// Loader for Cenneo OS
// /boot/arch/x86/interrupt/handle.c
// Interruption's handle funcions in C language

#include <lib/mem.h>
#include <mmu.h>
#include "../include/types.h"
#include "../include/interrupt.h"

// IDT地址及大小
X86Addr IDT_base;
#define		IDT_size	8 * 256

// Initialize
void init_interrupt(void)
{
	/**若分配内存不成功则输出错误信息*/
	IDT_base = bmalloc(IDT_size);

	/**初始化中断描述符表*/
	IDTR_write(IDT_base, IDT_size - 1);
	memset(IDT_base, 0, IDT_size);

	/**创建中断描述符*/
	create_ID(0x20 , 0x08, &int_0x20 , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x21 , 0x08, &int_0x21 , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x22 , 0x08, &int_0x22 , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x23 , 0x08, &int_0x23 , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x24 , 0x08, &int_0x24 , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x25 , 0x08, &int_0x25 , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x26 , 0x08, &int_0x26 , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x27 , 0x08, &int_0x27 , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x28 , 0x08, &int_0x28 , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x29 , 0x08, &int_0x29 , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x2A , 0x08, &int_0x2A , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x2B , 0x08, &int_0x2B , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x2C , 0x08, &int_0x2C , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x2D , 0x08, &int_0x2D , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x2E , 0x08, &int_0x2E , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x2F , 0x08, &int_0x2F , IDT_ITRP + IDT_32 + IDT_DPL_0 + IDT_P);
}
