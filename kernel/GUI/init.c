/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Initialization
 * Explorer/GUI/init.c
 * version:Alpha
 * 8/16/2014 9:08 AM
 */

#include <kmm.h>
#include <stdlib.h>
#include <GUI.h>
#include <lib/mem.h>
#include <lib/graphics.h>
#include <info.h>
#include <video.h>
#include "layer.h"

/**GUI根图层*/
struct layer *layer_root = NULL;
#define LAYER_ROOT_LENGTH	296
#define LAYER_ROOT_WIDTH	48

// Info output
#define GUI_INFO_BUF_LEN 64
char buf[GUI_INFO_BUF_LEN];

/**initialization GUI*/
void init_GUI(void)
{
	// Close kvi
	kvi_disable();

	/**清除屏幕*/
	clear_screen();

	/**创建GUI根图层*/
	for (;layer_root == NULL;) layer_root = kmalloc(sizeof(struct layer), 0);

	/**清空获得的内存*/
	memset(layer_root, 0, sizeof(struct layer));

	/**填充相应数据*/
	for (;layer_root->buf == NULL;) layer_root->buf = vmalloc (LAYER_ROOT_LENGTH * LAYER_ROOT_WIDTH * 4, 0);
	layer_root->visiable = true;
	layer_root->x = 0;
	layer_root->y = 0;
	layer_root->width = LAYER_ROOT_LENGTH;
	layer_root->height = LAYER_ROOT_WIDTH;
	layer_root->top = layer_root;
	layer_root->bottom = layer_root;
	layer_root->winptr = NULL;

	/**填充矩形*/
	GUI_put_square(layer_root, 0x400000ff, 0, 0, LAYER_ROOT_LENGTH, LAYER_ROOT_WIDTH);

	/**信息输出*/
	sprintf(buf, "Resolution:%ldx%ld %ldbits", vbe_info.xres, vbe_info.yres, vbe_info.bpp);
	GUI_put_string(layer_root, 0xffffffff, 0, 00, 0, 0, font("simsun"), "Graphical User Interface");
	GUI_put_string(layer_root, 0xffffffff, 0, 16, 0, 0, font("simsun"), buf);
	GUI_put_string(layer_root, 0xffffffff, 0, 32, 0, 0, font("simsun"), COPYRIGHT);

	GUI_update(layer_root);

	/**This function in window.c*/
	init_Window();
}

/**禁用GUI*/
void disable_GUI(void)
{

}
