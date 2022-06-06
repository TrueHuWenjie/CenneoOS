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

#include "begin.h"

void os_terminate(void)
{
	printk("OS Terminated. Press 'Enter' to shutdown, 'R' to reset.\n");
	while (1)
	{
		switch (get_key())
		{
			case 'r':
			case 'R':
				reset();
			case '\r':
				poweroff();
			default:
				break;
		}
	}
	
}

/**内核主函数*/
void main(void)
{
	// Kernel Memory Management
	init_kmm();

	init_urm();

	urm_create_dir("/", "kernel");

	// Kernel Log System
	init_kls();

	urm_create_dir("/", "devices");

	init_gfx();
	
	init_graph();

	urm_create_dir("/", "system");

	urm_create_dir("/", "drivers");
	urm_create_dir("/", "library");

	// Font
	init_font();

	// Kernel Visual Interface
	init_kvi();

	// Time
	init_time();

	// Multiple Process and Task
	init_mpt();

	// Programmable Interruption Controller
	init_PIC();

	// Programmable Interruption Timer
	init_PIT();

	// CPU
	init_CPU();

	// Output information about CPU
	output_CPU_info();

	// PS/2 Controller
	init_i8042();
	
	// Hard Disk Device
	init_ide();
	init_FAT32();

	lwip_init();
	dhcp_coarse_tmr();
	settimer(dhcp_coarse_tmr, 1000 * 60, 0);
	settimer(dhcp_fine_tmr, 500, 0);

	init_acpi();

	init_pci();

	InitRtl8139Driver();

	// Idle now
	task_name("Idle");

	switch (ebi.mode)
	{
		case BOOT_MODE_DESKTOP:
			begin_desktop_mode();
			break;

		case BOOT_MODE_EMBEDDED:
			begin_embedded_mode();
			break;

		case BOOT_MODE_SERVER:
			begin_server_mode();
			break;
	}
	
	os_terminate();
}
