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

/**BCD码转换成16进制方法*/
#define BCD_HEX(n)	((n >> 4) * 10) + (n & 0xf)

/**CMOS的地址空间没有映射到内存区域上，
 * 要读取CMOS，需要操作端口0x70、0x71
 * 先往0x70发送指定字节的偏移地址，
 * 再向0x71读、写，以读取、写入CMOS。
 */

// Get info from COMS
unsigned char cmos_read(unsigned char offset)
{
	io_out8(CMOS_INDEX, offset);		/**写入想要读出的信息在CMOS的偏移*/
	return io_in8(CMOS_DATA);		/**读出相关信息*/
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
	ktime.year = BCD_HEX(cmos_read(CMOS_CUR_YEAR));		/**当前年*/
	ktime.century = BCD_HEX(cmos_read(CMOS_CUR_CEN));		/**当前世纪*/
	/**CMOS中的年份仅仅是从本世纪开始到现在的绝对值，需要加上世纪*/
	ktime.year += ktime.century * 100;
}
