// Cenneo OS
// /kernel/include/lib/gpi.h
// Graphical Programming Library

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <lib/fonts/font.h>

/**绘制像素函数*/
void (*putpixel)(unsigned int x, unsigned int y, unsigned int color);

/**获取像素函数*/
unsigned int (*getpixel)(unsigned int x, unsigned int y);

void init_graph(void);
void outtextxy(unsigned long x, unsigned long y, unsigned int color, unsigned char *string);
void draw_font(unsigned long x, unsigned long y, unsigned int color, struct font_info *font_info, unsigned char ascii);
void rectangle(unsigned long x, unsigned long y, unsigned long width, unsigned long height, unsigned int color);
void line(unsigned long x0, unsigned long y0, unsigned long x1, unsigned long y1, unsigned int color);

/**24位色彩模式获取像素函数*/
unsigned int getpixel24(unsigned int x, unsigned int y);

/**32位色彩模式获取像素函数*/
unsigned int getpixel32(unsigned int x, unsigned int y);

/**24位色彩模式绘制像素函数*/
void putpixel24(unsigned int x, unsigned int y, unsigned int color);

/**32位色彩模式绘制像素函数*/
void putpixel32(unsigned int x, unsigned int y, unsigned int color);

void clear_screen(void);

#endif
