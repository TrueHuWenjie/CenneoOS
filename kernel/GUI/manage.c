/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Management
 * Explorer/GUI/manage.c
 * version:Alpha
 * 8/14/2014 1:59 PM
 */

#include <kmm.h>
#include <stdlib.h>
#include <GUI.h>
#include <lib/mem.h>
#include <lib/graphics.h>
#include "layer.h"
#include "window/unit.h"

/**创建新图层函数*/
struct layer *gui_new_layer(long x, long y, unsigned long width, unsigned long height)
{
	struct layer *new_layer;

	/**参数判断*/
	if (width == 0 | height == 0) return NULL;

	/**分配内存给图层结构体*/
	for (new_layer = NULL; new_layer == NULL;)
	new_layer = kmalloc(sizeof(struct layer), 0);

	/**结构体清空，防止意外参数干扰*/
	memset(new_layer, 0, sizeof(struct layer));

	/**加入双向链表中*/
	new_layer->top = layer_root->top;
	new_layer->bottom = layer_root;
	new_layer->top->bottom = new_layer;
	layer_root->top = new_layer;

	/**设置图层位置*/
	gui_set_position(new_layer, x, y);

	/**设置图层分尺寸*/
	gui_set_resolution(new_layer, width, height);

	/**正常返回*/
	return new_layer;
}

/**释放图层函数
 * target是要释放的图层
 * 该函数不可释放根图层！
 * 返回值：0 - 释放成功，1 - 释放失败
 */
int gui_free_layer(struct layer *target)
{
	/**判断要释放的图层是否是根图层*/
	if (target == layer_root) return -1;

	/**释放图层中的其他资源*/
	struct layer_unit *unit_ptr, *unit_next_ptr;

	/**释放图层单元资源*/
	for (unit_ptr = NULL, unit_next_ptr = target->layer_unit_list; unit_next_ptr != NULL; unit_next_ptr = unit_next_ptr->next)
	{
		/**释放前一个资源*/
		if (unit_ptr != NULL) kfree(unit_ptr);

		/**指向下一个资源*/
		unit_ptr = unit_next_ptr;
	}

	/**释放最后一个资源，可能有最后资源，也可能没有，但不用担心，kfree不会执行NULL指针释放*/
	kfree(unit_next_ptr);

	/**释放图层画布，画布可能没有，但是vfree能够正常处理*/
	// vfree(target->buf);

	/**注意，winptr可能会指向一个窗体结构，但是窗体结构的释放不属于本函数的工作！*/

	/**前后图层连接*/
	target->top->bottom = target->bottom;
	target->bottom->top = target->top;

	/**局部刷新*/
	gui_refresh_block(target->x, target->y, target->width, target->height);

	/**释放图层资源*/
	kfree(target);

	/**正常返回*/
	return 0;
}

/**
 * 改变图层高度函数(未正常运行)
 * 将target图层设置为height高度
 * 返回值 = 0：设置无效(已经是该高度)
 * 返回值 = 1：根图层(无意义)
 * 返回值为其他值时，代表正常返回，成功改变
 */
unsigned long gui_set_height(struct layer *target, unsigned long int height)
{
	unsigned long total = 0, retval;
	struct layer *layer = layer_root;

	/**先判断总共有多少个图层*/
	do{
		layer = layer->bottom;
		total ++;
	}while (layer != layer_root);

	/**要将target图层设置为height，
	 * 限制height最小值不得小于1
	 * 限制height最大不得超过(total-1)
	 * 最高图层是layer_root图层，高度为total
	 */

	if (height < 1) return 0;
	if (height >= total) height = (total - 1);

	//height = 5;

	/**目标高度不得等于该图层现在的高度*/
	unsigned long n = 1;
	for (layer = layer_root->top; layer != target; layer = layer->top) n ++;

	/**判断高度是否重复，如果是则意外返回*/
	if (n == height) return 0;
	//error("asd");
	/**确定了设置的高度*/
	retval = height;

	/**将目标高度位置记录下来*/
	struct layer *top, *bottom;
	height --;
	for (layer = layer_root->top; height != 0; layer = layer->top) height --;
	bottom = layer->bottom;
	top = layer;

	/**将target图层从双向链表中移除*/
	target->top->bottom = target->bottom;
	target->bottom->top = target->top;

	/**将target图层加入到双向链表中新的位置*/
	target->top = top;
	target->bottom = bottom;
	top->bottom = target;
	bottom->top = target;

	/**正常返回*/
}

/**获取图层高度函数
 * 返回值 = 0：获取失败(无该图层)
 */
unsigned long gui_get_height(struct layer *target)
{
	unsigned long retval = 0;
	struct layer *layer;

	/**循环计算*/
	layer = layer_root->top;

	do{
		/**计数加一*/
		retval ++;

		/**判断是否找到*/
		if (layer == target) return retval;

		/**向上搜寻*/
		layer = layer->top;
	/**排除无该图层的情况*/
	}while ((layer != layer_root) | (target == layer_root));

	/**运行到这里就是无图层的情况*/
	return 0;
}

/*change the position of the layer*/
long int gui_set_position(struct layer *layer, long x, long y)
{
	/**先保存旧的坐标信息*/
	long old_x = layer->x, old_y = layer->y;

	/**赋值新的坐标信息*/
	layer->x = x;
	layer->y = y;

	/**判断新坐标和旧坐标是否重叠*/
	if ((old_x == x) & (old_y == y)) return;

/**判断新坐标和旧坐标的关系，根据关系分块刷新，减少重复刷新的区域*/

	/**如果一个区域在另一个区域的上方或下方*/
	if (old_x == x)
	{
	/**判断谁在上面*/

		/**老区域在上面*/
		if (old_y < y)
		{
		/**判断是否有重叠*/

			/**有重叠的情况*/
			if (y < (old_y + layer->height))
			{
				/**一体刷新*/
				gui_refresh_block(old_x, old_y, layer->width, ((y - old_y) + layer->height));

			/**无重叠的情况*/
			}else{
				/**分别刷新*/
				gui_refresh_block(old_x, old_y, layer->width, layer->height);
				gui_refresh_block(x, y, layer->width, layer->height);
			}

		/**老区域在下边*/
		}else{
		/**判断是否有重叠*/

			/**有重叠的情况*/
			if (old_y < (y + layer->height))
			{
				/**一体刷新*/
				gui_refresh_block(x, y, layer->width, ((old_y - y) + layer->height));

			/**无重叠的情况*/
			}else{
				/**分别刷新*/
				gui_refresh_block(old_x, old_y, layer->width, layer->height);
				gui_refresh_block(x, y, layer->width, layer->height);
			}
		}
	}else{
		/**刷新相关区域*/
		gui_refresh_block(old_x, old_y, layer->width, layer->height);
		gui_refresh_block(x, y, layer->width, layer->height);
	}
}

/*change the resolution of the layer*/
long int gui_set_resolution(struct layer *layer, unsigned long width, unsigned long height)
{
	unsigned long ptr;

	/**判断当前图层的大小和目标大小是否一致*/
	if ((width == layer->width) && (height == layer->height))
		goto clean;

	/**如果该图层之前还有画布，首先要释放该画布*/
	if (layer->buf != NULL)
		vfree(layer->buf);

	/**分配新的内存*/
	for (layer->buf = NULL; layer->buf == NULL;)
		layer->buf = vmalloc(width * height * 4, 0);

	/**修改相关参数*/
	layer->width = width;
	layer->height = height;

clean:
	/**对新画布进行清空*/
	for (ptr = 0; ptr < (width * height); ptr ++)
		layer->buf[ptr] = 0x00000000;

	/**正常返回*/
	return 0;
}

/**
 * 图层贴图函数
 * 向layer图层以flag方式在位置(x,y)贴上图片(在length和width范围之内贴图)
 */
long int gui_map(struct layer *layer, struct gui_image *image, unsigned long x, unsigned long y, unsigned long width, unsigned long height, int flag)
{
	unsigned int *data = image->data;
	unsigned long offset_x, offset_y;

	/**检查image和layer是否为空指针*/
	if (layer == NULL) return -1;
	if (image == NULL) return -1;

	/**判断length或者width是否为0，只要为0则按图片大小标准拷贝*/
	if (width == 0) width = image->width;
	if (height == 0) height = image->height;

	/**判断layer和图片要拷贝的位置大小信息，选其中最小值（防止溢出）*/
	if ((x + width) > layer->width)
		width = layer->width - x;
	if ((y + height) > layer->height)
		height = layer->height - y;

	/**纵坐标循环复制数据*/
	for (offset_y = 0;; offset_y ++)
	{
		/**判断是否超过复制的限定\图形范围*/
		if ((offset_y >= height) | (offset_y >= image->height)) break;

		/**横坐标循环复制数据*/
		for (offset_x = 0;; offset_x ++)
		{
			/**判断是否超过复制的限定范围*/
			if ((offset_x >= width) | (offset_x >= image->width)) break;

			/**数据复制*/
			layer->buf[(offset_y + y) * layer->width + offset_x + x] = data[(offset_y * image->width) + offset_x];
		}
	}

	/**对图层块刷新*/
	gui_refresh_block(layer->x, layer->y, layer->width, layer->height);
}
