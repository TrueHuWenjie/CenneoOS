/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Function main
 * Explorer/init/main.c
 * version:Alpha
 * 7/9/2014 8:30 AM
 */

/**
 * Explorer操作系统内核初始化主函数
 * 进行相关的初始化工作
 */

#include <lib/mem.h>
#include <lib/graphics.h>
#include <stdarg.h>
#include <stdlib.h>
#include <../fs/fat32/fat32.h>
#include <GUI.h>
#include <main.h>
#include <types.h>
#include <task.h>
#include <arch.h> // Architecture - 架构层
#include <keyboard.h>
#include <kmm.h>
#include <video.h>
#include <kvi.h>

/**在_start.asm中放置了boot_info_ptr指针指向boot_info*/
extern struct boot_info *boot_info_ptr;

/**内核主函数*/
void main(void)
{
	init_kmm();

	init_graph();
	init_font();
	init_kvi();

	init_time();
	init_task();
	init_CPU();
	init_PIC();
	init_PIT();

	/**打印信息*/
	output_CPU_info();			// 打印处理器信息

	/**初始化设备*/
	init_keyboard();			// 初始化键盘
	init_mouse();				// 初始化鼠标
	init_hdd();					// 初始化磁盘
	init_FAT32();				// 初始化FAT32文件系统

	/**GUI初始化*/
	init_GUI();

	// Operating System Moniter
	extern int osm_open(void);
	new_task(&osm_open, NULL);
}
