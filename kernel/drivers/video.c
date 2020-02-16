/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Standard Video Support
 * Explorer/drivers/video.c
 * version:Alpha
 * 7/10/2014 8:30 AM
 */

#include "../arch/x86/include/x86ebi.h"
#include <types.h>
#include <video.h>

struct vbe_info vbe_info;

/**Initialize for vesa*/
void init_vesa(void)
{
	vbe_info.xres = ebi.ModeInfoBlock.XResolution;
	vbe_info.yres = ebi.ModeInfoBlock.YResolution;
	vbe_info.bpp = ebi.ModeInfoBlock.BitsPerPixel;
	vbe_info.vram_length = (((vbe_info.xres * vbe_info.yres) * (vbe_info.bpp / 8)) & 0xfffff000) + 0x1000;
	vbe_info.vram = ebi.ModeInfoBlock.PhysBasePtr;
	//vbe_info.vram = vmalloc(vbe_info.vram_length);
	//kmap(vbe_info.vram, ebi.ModeInfoBlock.PhysBasePtr, vbe_info.vram_length);
}
