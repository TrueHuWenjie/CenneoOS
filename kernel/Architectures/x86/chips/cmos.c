/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Rose<roselyu@163.com> Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer CMOS support
 * Explorer/arch/x86/kernel/cmos.c
 * version:Aplha
 * 12/6/2014 8:58 PM
 */

#include "../include/io.h"
#include "../include/cmos.h"
#include <time.h>

// Ports
#define CMOS_PORT_INDEX		0x70
#define CMOS_PORT_DATA		0x71

// BCD to HEX
#define BCD_HEX(n)	bcd2hex(n)

static char bcd2hex(char bcd)
{
	return ((bcd >> 4) * 10) + (bcd & 0x0f);
}

// Get info from COMS
unsigned char cmos_read(unsigned char offset)
{
	io_out8(CMOS_PORT_INDEX, offset);
	return io_in8(CMOS_PORT_DATA);
}

void cmos_readtime(void)
{
	/**获取CMOS芯片上的年、月、日、时、分、秒*/
	ktime.second = BCD_HEX(cmos_read(CMOS_CUR_SEC));			/**当前秒*/
	ktime.mintus = BCD_HEX(cmos_read(CMOS_CUR_MIN));			/**当前分*/
	ktime.hour = BCD_HEX(cmos_read(CMOS_CUR_HOUR));		/**当前时*/
	ktime.week_day = BCD_HEX(cmos_read(CMOS_WEEK_DAY));	/**一周中当前天*/
	ktime.day = BCD_HEX(cmos_read(CMOS_MON_DAY));			/**一月中当前日*/
	ktime.month = BCD_HEX(cmos_read(CMOS_CUR_MON));			/**当前月*/
	ktime.year = BCD_HEX(cmos_read(CMOS_CUR_YEAR)) + \
	BCD_HEX(cmos_read(CMOS_CUR_CEN)) * 100;
}
