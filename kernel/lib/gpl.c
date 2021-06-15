// Cenneo OS
// /kernel/lib/gpi.c
// Graphical Programming Library

#include <lib/mem.h>
#include <lib/fonts/font.h>
#include <lib/math.h>
#include <video.h>
#include <lib/gpl.h>

/**显示像素函数*/
void (*putpixel)(unsigned int x, unsigned int y, unsigned int color);

/**获取像素函数*/
unsigned int (*getpixel)(unsigned int x, unsigned int y);

/**graphical initialization*/
void init_graph(void)
{
	extern void init_vesa(void);

	/**初始化VBE模式*/
	init_vesa();

	/**根据BPP判断所需要的相应显示像素函数*/
	if (vbe_info.bpp == 24)
	{
		putpixel = putpixel24;
		getpixel = getpixel24;
	}else if (vbe_info.bpp == 32)
	{
		putpixel = putpixel32;
		getpixel = getpixel32;
	}
}

/**put a string in screen*/
void outtextxy(unsigned long x, unsigned long y, unsigned int color, unsigned char *string)
{
	unsigned long point;
	/**获得字库信息*/
	struct font_info *font_info;
	font_info = font("Standard Font");
	for (point = 0; string[point] != 0x00; point ++)
	{
		draw_font(x, y, color, font_info, string[point]);
		x += 8;
	}
}

/*显示字*/
void draw_font(unsigned long x, unsigned long y, unsigned int color, struct font_info *font_info, unsigned char ascii)
{
	unsigned long p, i, font_offset;/*字库偏移量*/
	unsigned char d;
	font_offset = ascii * 16;
	for (i = 0; i < 16; i++)
	{
		d = font_info->addr[font_offset + i];
		if ((d & 0x80) != 0) { putpixel(x, y + i, color); }
		if ((d & 0x40) != 0) { putpixel(x + 1, y + i, color); }
		if ((d & 0x20) != 0) { putpixel(x + 2, y + i, color); }
		if ((d & 0x10) != 0) { putpixel(x + 3, y + i, color); }
		if ((d & 0x08) != 0) { putpixel(x + 4, y + i, color); }
		if ((d & 0x04) != 0) { putpixel(x + 5, y + i, color); }
		if ((d & 0x02) != 0) { putpixel(x + 6, y + i, color); }
		if ((d & 0x01) != 0) { putpixel(x + 7, y + i, color); }
	}
}

/*在屏幕指定地方画方块*/
void rectangle(unsigned long x, unsigned long y, unsigned long width, unsigned long height, unsigned int color)
{
	unsigned long m, n;
	for (n = 0; n != height; n ++)
	{
		for (m = 0; m != width; m ++)
		{
			putpixel(x + m, y + n, color);
		}
	}
}

void line(unsigned long x0, unsigned long y0, unsigned long x1, unsigned long y1, unsigned int color)
{
	int dx, dy, n, k, i, f;
	int x, y;
	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	n = dx + dy;

	/**根据x1和x0的关系，选择合适的k运算方法*/
	if (x1 == x0)
	{
		k = 2;
		x = x0;
		y = y0;
	}else if (x1 >= x0)
	{
		k = y1 >= y0 ? 1 : 4;
		x = x0;
		y = y0;
	}else{
		k = y1 >= y0 ? 2 : 4;
		x = x0;
		y = y0;
	}

	for (i = 0, f = 0; i < n; i ++)
		if (f >= 0)
		switch (k)
		{
			case 1:putpixel(x++, y, color);f -= dy;break;
			case 2:putpixel(x, y++, color);f -= dx;break;
			case 3:putpixel(x--, y, color);f -= dy;break;
			case 4:putpixel(x, y--, color);f -= dx;break;
		}else
		switch(k)
		{
			case 1:putpixel(x, y++, color);f += dx;break;
			case 2:putpixel(x--, y, color);f += dy;break;
			case 3:putpixel(x, y--, color);f += dx;break;
			case 4:putpixel(x++, y, color);f += dy;break;
		}
}

/**24位色彩模式获取像素函数*/
unsigned int getpixel24(unsigned int x, unsigned int y)
{
	unsigned int i;

	/**先判断该像素是否在屏幕上*/
	if (x < vbe_info.xres && y < vbe_info.yres)
	{
		i = ((y * vbe_info.xres) + x) * 3;
		return (vbe_info.vram[i] + (vbe_info.vram[i+1] << 8) + (vbe_info.vram[i+2] << 16));
	}
}

/**24位色彩模式获取像素函数*/
unsigned int getpixel32(unsigned int x, unsigned int y)
{
	/**先判断该像素是否在屏幕上*/
	if (x < vbe_info.xres && y < vbe_info.yres)
	{
		return ((unsigned int *)vbe_info.vram)[(y * vbe_info.xres) + x];
	}
}

/**24位色彩模式绘制像素函数*/
void putpixel24(unsigned int x, unsigned int y, unsigned int color)
{
	int i;
	unsigned char *vram = vbe_info.vram;

	/**先判断该像素是否在屏幕上*/
	if ((x < vbe_info.xres) & (y < vbe_info.yres))
	{
		i = ((y * vbe_info.xres) + x)*3;
		vram[i] = color;
		vram[i+1] = color >> 8;
		vram[i+2] = color >> 16;
	}
}

void putpixel32(unsigned int x, unsigned int y, unsigned int color)
{
	/**先判断该像素是否在屏幕上*/
	if ((x < vbe_info.xres) & (y < vbe_info.yres))
	{
		((unsigned int *)vbe_info.vram)[(y * vbe_info.xres) + x] = color;
	}
}

/**清除屏幕函数*/
void clear_screen(void)
{
	memset(vbe_info.vram, 0, vbe_info.vram_length);
}
