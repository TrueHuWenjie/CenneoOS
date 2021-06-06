/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface head
 * Explorer/include/GUI.h
 * version:Alpha
 * 8/16/2014 4:59 PM
 */

#ifndef gui_H_
#define gui_H_

#include <lib/fonts/font.h>
#include <stdbool.h>
#include "../GUI/layer.h"

/**GUI/init.c中*/
void init_gui(void);

/**GUI/manage.c中*/
struct layer *gui_new_layer(long x, long y, unsigned long width, unsigned long height);

/**释放图层函数*/
int gui_free_layer(struct layer *target);

/**
 * 改变图层高度函数
 * 将target图层设置为height高度
 * 返回值 = 0：设置无效(已经是该高度)
 * 返回值 = 1：根图层(无意义)
 * 返回值为其他值时，代表正常返回，成功改变
 */
unsigned long gui_layer_set_z(struct layer *target, unsigned long int height);

long int gui_set_position(struct layer *layer, long x, long y);
long int gui_set_resolution(struct layer *layer, unsigned long width, unsigned long height);

/**refresh.c中*/

// Update a whole layer
void gui_update(struct layer *target);

/**刷新像素函数
 * (x,y)是在屏幕上需要刷新的像素坐标
 * 该函数为通用函数，仅为兼容性考虑。
 * 在具体平台上建议用相应的汇编语言实现该函数以获得最大效能
 */
long int gui_refresh_pixel(unsigned long int x, unsigned long int y);

/**GUI块刷新函数*/
void gui_refresh_block(long x, long y, unsigned long width, unsigned long height);

/**GUI全部刷新*/
long int gui_refresh(void);

/**图层贴图函数
 * 向layer图层以flag方式在位置(x,y)贴上图片(在width和height范围之内贴图)
 */
long int gui_map(struct layer *layer, struct gui_image *image, unsigned long x, unsigned long y, unsigned long width, unsigned long height, int flag);

/**GUI Graphical Library*/
void gui_put_string(struct layer *layer, unsigned int color, unsigned long x, unsigned long y, unsigned long width, unsigned long height, struct font_info *font_info, const char *string);

/**put word to layer*/
void gui_put_word(struct layer *layer, unsigned int color, unsigned long x, unsigned long y, unsigned long width, unsigned long height, struct font_info *font_info, unsigned char ascii);

/**draw a line in the layer*/
void gui_line(struct layer *layer, unsigned int color, unsigned long x0, unsigned long y0, unsigned long x1, unsigned long y1);

/**绘制一个方块*/
long int gui_put_square(struct layer *layer, unsigned int color, unsigned long x, unsigned long y, unsigned long width, unsigned long height);

/**
 * 画一个像素至图层上
 * made by Lab Explorer<2322869088@qq.com>
 * layer为要画的目标图层，color为要绘制的像素颜色(32-bit)
 * (x,y)是像素的坐标，length和width是用于人为边界判断，0代表仅作图层边框判断
 *
 * 人为边界判断：窗口作为加入窗口元素的图层，在被画上数据时，容易因为无有效判断而
 * 绘制到窗口本身的部分。程序绘制控件的时候也容易画到其他控件的区域。为了解决这个
 * 问题，加入人为边界判断方法，对像素是否超出范围进行有效的判断。从防止绘制过界的
 * 角度出发，不光要有边界判断(因为边界判断仅仅判断是否会溢出到右边或下边)，还要
 * 在一开始就对整体绘制元素的坐标进行纠正，以不会溢出到左边和上边。
 */
void gui_put_pixel(struct layer *layer, unsigned int color, unsigned long x, unsigned long y, unsigned long width, unsigned long height);

/**获取一个图层的指定位置的像素颜色*/
unsigned int gui_get_pixel(struct layer *layer, unsigned long x, unsigned long y);

/**GUI/window/lib.c中*/

/**创建Window函数
 * 创建一个窗口，标题为title，
 * 风格为style，
 * 当x = 0且y = 0时，窗口会创建在最中间
 * 窗口中有效内容尺寸为length * height
 */
struct window *gui_window(char *title, char style, unsigned long x, unsigned long y, unsigned long width, unsigned long height);

/**输出字符串到窗口*/
void window_put_string(struct window *target, unsigned long x, unsigned long y, const char *s);

// Put character into Window
void window_put_char(struct window *target, unsigned int color, \
struct font_info *font_info, unsigned long x, unsigned long y, const char c);

/**窗口信息输出函数*/
int window_print(struct window *target, const char *fmt, ...);


#endif
