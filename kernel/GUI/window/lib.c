/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Window System Library
 * Explorer/GUI/window/lib.c
 * version:Alpha
 * 2/5/2015 1:37 PM
 */

#include <lib/string.h>
#include <lib/gpl.h>
#include <graphics.h>
#include <gui.h>
#include <kmm.h>
#include <stdlib.h>
#include <stdarg.h>
#include "window.h"
#include "unit.h"
#include "../layer.h"
#include "../recourse.h"

/**
 * Explorer内核图形化界面窗口系统库
 * 在GUI的图层化基础上的高级图形库
 */

/**创建Window函数
 * 创建一个窗口，标题为title，
 * 风格为style，
 * 当x = 0且y = 0时，窗口会创建在最中间
 * 窗口中有效内容尺寸为length*width
 */
struct window *gui_window(char *title, char style, unsigned long x, unsigned long y, unsigned long width, unsigned long height)
{
	unsigned long real_width, real_height;
	/**根据不同的风格，进行不同的初始化*/
	if (style == WINDOW_NONE)
	{
		/**无边框*/
		real_width = width;
		real_height = height;

		/**无边框窗口最小不得长宽为0*/
		if ((width * height) == 0)
		{
			/**错误返回*/
			return NULL;
		}
	}else if (style == WINDOW_NORMAL)
	{
		/**算进边框大小*/
		real_width = width + (WINDOW_NORMAL_FRAME_WIDTH * 2);
		real_height = height + WINDOW_NORMAL_HEADER_WIDTH + WINDOW_NORMAL_FRAME_WIDTH;

		/**正常边框窗口最小不得长宽为0*/
		if ((width * height) == 0)
		{
			/**错误返回*/
			return NULL;
		}
	}

	/**分配一个新的图层*/
	struct window *new_window;
	new_window = kmalloc(sizeof(struct window), 0);
	if (new_window == NULL) error("window create failed.");

	/**结构体赋值*/
	strncpy(new_window->title, title, WINDOW_NUM_TITLE);
	new_window->style = style;
	new_window->length = width;
	new_window->width = height;
	new_window->print_x = 0;
	new_window->print_y = 0;
	new_window->ident = WINDOW_IDENT;

	/**确定新窗口的位置
	 * 如果x,y都为0，则默认新窗口位于正中间
	 * 如果x,y至少有一个不为0，则新窗口位置在(x,y)
	 */
	if ((x == 0) & (y == 0))
	{
		x = ((vbe_info.xres - real_width) / 2);
		y = ((vbe_info.yres - real_height) /2);
	}

	/**创建、初始化图层*/
	new_window->layer = gui_new_layer(x, y, real_width, real_height);

	/**将该图层从图层链表中脱离*/
	new_window->layer->top->bottom = new_window->layer->bottom;
	new_window->layer->bottom->top = new_window->layer->top;

	/**将该图层置于任务栏之下，其他窗口之上*/
	new_window->layer->top = taskbar_layer;
	new_window->layer->bottom = taskbar_layer->bottom;
	taskbar_layer->bottom = new_window->layer;
	new_window->layer->bottom->top = new_window->layer;

	/**设置为可见*/
	new_window->layer->visiable = true;

	/**设置指针*/
	new_window->layer->winptr = new_window;

	/**判断风格是否是无风格*/
	if (style == WINDOW_NONE)
	{
		/**无边框*/
		gui_put_square(new_window->layer, 0xffffffff, 0, 0, width, height);

	/**判断是不是正常风格*/
	}else if (style == WINDOW_NORMAL)
	{
		/**设置新的窗口为活动窗口*/
		window_set_active(new_window);

		/**绘制按钮*/
		gui_map(new_window->layer, close_f_button, new_window->layer->width - close_f_button->width - WINDOW_NORMAL_FRAME_WIDTH, 0, 0, 0, 0);
		gui_map(new_window->layer, mini_f_button, new_window->layer->width - (close_f_button->width + mini_f_button->width) - WINDOW_NORMAL_FRAME_WIDTH, 0, 0, 0, 0);

		// /**建立单元*/
		gui_new_unit(new_window->layer, UNIT_CLOSE, new_window->layer->width - close_f_button->width - WINDOW_NORMAL_FRAME_WIDTH, 0, close_f_button->width, close_f_button->height);
		gui_new_unit(new_window->layer, UNIT_MINI, new_window->layer->width - (close_f_button->width + mini_f_button->width) - WINDOW_NORMAL_FRAME_WIDTH, 0, mini_f_button->width, mini_f_button->height);

		/**主体*/
		gui_put_square(new_window->layer, 0xffffffff, WINDOW_NORMAL_FRAME_WIDTH, WINDOW_NORMAL_HEADER_WIDTH, width, height);
	}

	/**返回新窗口的指针*/
	return new_window;
}

/**释放窗口函数*/
void gui_free_window(struct window *target)
{
	/**释放窗口使用的图层资源*/
	gui_free_layer(target->layer);

	/**释放窗口*/
	kfree(target);

	/**正常返回*/
	return;
}

/**纠正绘制参数函数
 * 窗体有无边框、边框风格不同时
 * 绘制到图层上的实际地址是不一样的
 */
void correct_para(struct window *target, unsigned long *x, unsigned long *y, unsigned long *width, unsigned long *height)
{
	/**根据窗口风格纠正参数*/
	if (target->style == WINDOW_NONE)
	{
		/**无边框*/
	}else if (target->style == WINDOW_NORMAL)
	{
		/**有窗体边框*/
		*x += WINDOW_NORMAL_FRAME_WIDTH;
		*y += WINDOW_NORMAL_HEADER_WIDTH;

	/**判断是否超过边界，如果超过，以边界为准*/
		if ((width != NULL) & (*width >= (target->length + WINDOW_NORMAL_FRAME_WIDTH)))
			*width = target->length + WINDOW_NORMAL_FRAME_WIDTH;

		if ((height != NULL) & (*height >= (target->width + WINDOW_NORMAL_HEADER_WIDTH)))
			*height = target->width + WINDOW_NORMAL_HEADER_WIDTH;
	}
	return;
}

/**输出字符串到窗口*/
void window_put_string(struct window *target, unsigned long x, unsigned long y, const char *s)
{
	unsigned long width = 0xffffffff, height = 0xffffffff;
	/**纠正参数*/
	correct_para(target, &x, &y, &width, &height);

	/**输出字符串*/
	gui_put_string(target->layer, 0xff000000, x, y, width, height, font("Standard Font"), s);
}

// Put character into Window
void window_put_char(struct window *target, unsigned int color, \
struct font_info *font_info, unsigned long x, unsigned long y, const char c)
{
	unsigned long width = 0xffffffff, height = 0xffffffff;
	/**纠正参数*/
	correct_para(target, &x, &y, &width, &height);

	/**输出字符串*/
	gui_put_word(target->layer, color, x, y, width, height, font_info, c);
}

/**窗口信息输出函数*/
int window_print(struct window *target, const char *fmt, ...)
{
	va_list arg;
	unsigned int n;

	/**缓冲区*/
	char buffer[64];

	/**初始化参数指针*/
	va_start(arg, fmt);

	/**格式化写入缓存并返回长度*/
	n = vsprintf(buffer, fmt, arg);

	/**输出字符串到窗口*/
	window_put_string(target, target->print_x, target->print_y, buffer);

	/**指针指向新区域*/
	target->print_y += 16;

	/**处理指针，防止误操作*/
	va_end(arg);
	return n;
}
