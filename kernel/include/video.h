/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Standard Video Support Definition
 * Explorer/include/video.h
 * version:Alpha
 * 7/10/2014 8:30 AM
 */

#ifndef VEDIO_H_
#define VEDIO_H_

struct vbe_info{
	unsigned char *vram;
	unsigned long xres, yres;
	unsigned long vram_length;
	unsigned long bpp;
};

extern struct vbe_info vbe_info;

void init_vesa(void);

#endif
