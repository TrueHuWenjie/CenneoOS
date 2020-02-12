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
#include <lib/graphics.h>
#include <video.h>

/**Initialize for vesa*/
void init_VESA(void)
{
	Video_Info.xres = ebi.ModeInfoBlock.XResolution;
	Video_Info.yres = ebi.ModeInfoBlock.YResolution;
	Video_Info.bit_per_pixel = ebi.ModeInfoBlock.BitsPerPixel;
	Video_Info.vram_length = (((Video_Info.xres * Video_Info.yres) * (Video_Info.bit_per_pixel / 8)) & 0xfffff000) + 0x1000;
	Video_Info.vram = (char *)ebi.ModeInfoBlock.PhysBasePtr;
	// Video_Info.vram = (unsigned char*)vmalloc(Video_Info.vram_length);

	// Video_Info.vram = (unsigned char*)ebi.ModeInfoBlock.PhysBasePtr;
	// kmap(Video_Info.vram, ebi.ModeInfoBlock.PhysBasePtr, Video_Info.vram_length);
}
