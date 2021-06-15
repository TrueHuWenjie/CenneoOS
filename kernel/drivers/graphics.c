// Cenneo OS
// /kernel/drivers/graphics.c
// Graphics Card support

#include "../arch/x86/include/x86ebi.h"
#include <types.h>
#include <kmm.h>
#include <graphics.h>

struct vbe_info vbe_info;

/**Initialize for vesa*/
void init_vesa(void)
{
	vbe_info.xres = ebi.ModeInfoBlock.XResolution;
	vbe_info.yres = ebi.ModeInfoBlock.YResolution;
	vbe_info.bpp = ebi.ModeInfoBlock.BitsPerPixel;
	vbe_info.vram_length = (((vbe_info.xres * vbe_info.yres) * (vbe_info.bpp / 8)) & 0xfffff000) + 0x1000;
	vbe_info.vram = vmalloc(vbe_info.vram_length, VM_MAPD + VM_WAIT);
	if (!vbe_info.vram) reset();
	vmap(ebi.ModeInfoBlock.PhysBasePtr, vbe_info.vram, vbe_info.vram_length);
}
