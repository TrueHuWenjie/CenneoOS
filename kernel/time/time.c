// Cenneo OS
// /kernel/time/time.c
// Time Management

#include "../arch/x86/include/cmos.h"
#include <time.h>
#include <types.h>
#include <kvi.h>

struct ktime_def ktime;

void init_time(void)
{
	// Read time from CMOS
	cmos_readtime();

	printk("time:century:%d year:%d month:%d day:%d week:%d hour:%d min:%d sec:%d\n", \
	ktime.century, ktime.year, ktime.month, ktime.day, ktime.week_day, ktime.hour, ktime.mintus, ktime.second);
}
