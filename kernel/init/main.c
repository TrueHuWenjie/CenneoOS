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
#include <arch.h> // Archtecture - 架构层
#include <keyboard.h>
#include <kmm.h>
#include <video.h>
#include <kvi.h>

/**在_start.asm中放置了boot_info_ptr指针指向boot_info*/
extern struct boot_info *boot_info_ptr;

/**任务测试函数*/
int task_test()
{
	unsigned long n;
	for (;;){
		for (n = 0; n < 0xffffff; n++);
		printk("process:%d.\n", get_id());
	}
}


/**内核主函数*/
void main(void)
{
	init_kmm();

	init_graph();
	init_font();

	// Open kvi
	kvi_open();

	kmm_info();
	int *ptr;
	ptr = vmalloc(8192, VM_MAPD);
	kmm_info();

fin2:goto fin2;
	init_time();
	init_task();
	init_CPU();
	init_PIC();
	init_PIT();

	/**打印信息*/
	// output_CPU_info();			// 打印处理器信息
	output_viedo_info();		// 打印VBE的相关信息

	/**初始化设备*/
	init_keyboard();			// 初始化键盘
	init_mouse();				// 初始化鼠标
	init_hdd();					// 初始化磁盘
	init_FAT32();				// 初始化FAT32文件系统

	/**多任务性能测试*/
	// new_task(&task_test, NULL);

	printk("Hello, world!asfasfd");

	/**GUI初始化*/
	//init_GUI();
}

/**输出VBE信息*/
void output_viedo_info(void)
{
	//struct VbeInfoBlock *VbeInfoBlock = &ebi.VbeInfoBlock;

	/**空出一行*/
	//printk("\n");

	/**视频控制器信息*/
	//printk("Video Controler:%s,Version:%d.%d.\n", VbeInfoBlock->VbeSignature, (VbeInfoBlock->VbeVersion >> 8), VbeInfoBlock->VbeVersion & 0xff);

	/**品牌*/
	//printk("OEM:%s.\n", real_addr_in_pm(VbeInfoBlock->OemStringPtr_Seg, VbeInfoBlock->OemStringPtr_Off));

	/**详细屏幕信息*/
	//printk("XResolution:%d, YResolution:%d, BitsPerPixel:%d.\n", ebi.ModeInfoBlock.XResolution, ebi.ModeInfoBlock.YResolution, ebi.ModeInfoBlock.BitsPerPixel);

	/**显存信息*/
	//printk("PhysBasePtr:%#X, Size of vram:%dBytes, vitual address of vram in kernel:%#X.\n", ebi.ModeInfoBlock.PhysBasePtr, vbe_info.vram_length, vbe_info.vram);
}
