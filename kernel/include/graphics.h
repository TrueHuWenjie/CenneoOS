// Cenneo OS
// /kernel/include/graphics.h
// Graphics Card support

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

struct vbe_info{
	unsigned char *vram;
	unsigned long xres, yres;
	unsigned long vram_length;
	unsigned long bpp;
};

extern struct vbe_info vbe_info;

void init_vesa(void);

#endif
