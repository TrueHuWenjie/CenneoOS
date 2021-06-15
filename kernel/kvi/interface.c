// Cenneo OS
// /kernel/kvi/interface.c
// Kernel-support visual interface

#include "../include/kvi.h"
#include "theme.h"
#include <stdbool.h>
#include <graphics.h>
#include <lib/gpl.h>
#include <info.h>

#define WIDTH_MAX 100
#define HEIGHT_MAX 37
#define SCR_W kvi_sm.xres
#define SCR_H kvi_sm.yres
#define BYTEPERPIXEL 3
#define FONT_W 8
#define FONT_H 16
#define SIZE_OF_BUFFER 256

// kvi's state machine
struct kvi_sm
{
	u32 xres;
	u32 yres;
	u32 cursor_column;
	u32 cursor_row;
	u32 width;
	u32 height;
	u32 bg_color;
	u32 fg_color;
	bool enable;
} kvi_sm = {.enable = false};

// put a char on screen
void kvi_put_char(char ascii)
{
	/**如果shell当前不可用，就直接返回*/
	if (kvi_sm.enable == false) return;

	/**对控制字符的判断*/
	if (ascii < 0x20)
	{
		/**如果输入字符是换行符，则进行换行*/
		if(ascii == 0xA)
		{
			kvi_sm.cursor_column = 0;
			kvi_sm.cursor_row ++;
		}

	/**如果不是控制字符*/
	}else{
		/**在光标处位置显示字*/
		draw_font(kvi_sm.cursor_column * FONT_W, kvi_sm.cursor_row * FONT_H, \
			kvi_sm.fg_color, font(DEFAULT_FONT), ascii);

		/**将光标移动到下一个位置*/
		kvi_sm.cursor_column ++;
	}

	/**判断是否需要换行*/
	if (kvi_sm.cursor_column >= kvi_sm.width)
	{
		kvi_sm.cursor_column = 0;
		kvi_sm.cursor_row ++;
	}

	if (kvi_sm.cursor_row >= kvi_sm.height)
	{
		kvi_roll_screen();
	}
}

// Roll Screen
void kvi_roll_screen(void)
{
	unsigned long x, y;

	/**循环将第一行之外的其他行向上提一行*/
	for (y = 16; y < kvi_sm.yres; y ++)
	{
		for (x = 0; x < kvi_sm.xres; x ++)
		{
			putpixel(x, y - 16, getpixel(x, y));
		}
	}

	/**未结束*/
	for (y = (kvi_sm.yres - ((kvi_sm.yres % 16) + 16)); y < kvi_sm.yres; y ++)
	{
		for (x = 0; x < kvi_sm.xres; x ++)
		{
			putpixel(x, y, kvi_sm.bg_color);
		}
	}

	kvi_sm.cursor_row --;
}

void kvi_color(u32 fg_color, u32 bg_color)
{
	kvi_sm.fg_color = fg_color;
	kvi_sm.bg_color = bg_color;
}

// Basic info output
void kvi_basicinfo(void)
{
	printk("Kernel Name:" KERNEL_NAME " ");
	printk(KERNEL_VERSION "\n");
	printk("Kernel Author:" AUTHOR "\n");
	printk(COPYRIGHT "\n");

	/**其他信息的输出*/
	if (BASE) printk("It is based on " BASE ".\n");
}

// Close kvi
void kvi_disable(void)
{
	kvi_sm.enable = false;
}

// Open kvi
void kvi_enable(void)
{
	kvi_sm.enable = true;
}

// Initialization for kvi
void init_kvi(void)
{
	// Enable kvi
	kvi_enable();

	kvi_sm.xres = vbe_info.xres;
	kvi_sm.yres = vbe_info.yres;

	// Clear the screen
	rectangle(0, 0, kvi_sm.xres, kvi_sm.yres, kvi_sm.bg_color);
	kvi_sm.cursor_column = 0;
	kvi_sm.cursor_row = 0;

	/**计算屏幕长度、宽度*/
	kvi_sm.width = kvi_sm.xres / FONT_W;
	kvi_sm.height = kvi_sm.yres / FONT_H;

	kvi_color(KVI_NOTE_FGCOLOR, KVI_NOTE_BGCOLOR);

	/**打开shell*/
	kvi_sm.enable = true;

	/**打印内核信息*/
	kvi_basicinfo();
}
