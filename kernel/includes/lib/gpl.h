// Cenneo OS
// /kernel/include/lib/gpi.h
// Graphical Programming Library

#ifndef GPL_H_
#define GPL_H_

#include <lib/font.h>

struct gpl_descriptor_def
{
    void *buf;
    unsigned long xres;
    unsigned long yres;
    unsigned char bpp;
};

/**绘制像素函数*/
extern void (*putpixel)(unsigned int x, unsigned int y, unsigned int color);

/**获取像素函数*/
extern unsigned int (*getpixel)(unsigned int x, unsigned int y);

void init_graph(void);
void outtextxy(unsigned long x, unsigned long y, unsigned int color, unsigned char *string);
void draw_font(unsigned long x, unsigned long y, unsigned int color, struct font_info *font_info, unsigned char ascii);
void rectangle(unsigned long x, unsigned long y, unsigned long width, unsigned long height, unsigned int color);
void line(unsigned long x0, unsigned long y0, unsigned long x1, unsigned long y1, unsigned int color);

void clear_screen(void);

#endif
