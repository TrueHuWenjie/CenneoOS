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
#include <mpt.h>
#include <arch.h> // Architecture - 架构层
#include <keyboard.h>
#include <kmm.h>
#include <video.h>
#include <kvi.h>

/**内核主函数*/
void main(void)
{
	// Kernel Memory Management
	init_kmm();

	// Kernel Log System
	init_kls();

	init_graph();

	// Font
	init_font();

	// Kernel Visual Interface
	init_kvi();

	// Time
	init_time();

	// Multiple Process and Task
	init_mpt();

	// CPU
	init_CPU();

	// Programmable Interruption Controller
	init_PIC();

	// Programmable Interruption Timer
	init_PIT();

	// Output information about CPU
	output_CPU_info();

	// Keyboard
	init_keyboard();

	// Mouse
	init_mouse();

	// Hard Disk Device
	init_hdd();
	init_FAT32();

	// Graphical User Interface
	init_GUI();

	// Operating System Moniter
	extern int osm_open(void);
	task(&osm_open, NULL);

	// Kernel Log System Monitor
	extern int klsm_open(void);
	task(&klsm_open, NULL);

	// Idle now
	task_name("Idle");

	while (1)
	{
		io_hlt();
	}
}
