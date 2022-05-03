// Cenneo OS
// /kernel/time/time.c
// Time Management

#include "../Architectures/x86/include/cmos.h"
#include <time.h>
#include <types.h>
#include <lib/url.h>
#include <urm.h>
#include <kvi.h>

// Kernel time
struct ktime_def ktime;

// Current kernel's runtime
unsigned long sys_now(void)
{
	return ktime.system_runtime;
}

struct ktime_def time_s2t(void)
{
	
}

void init_time(void)
{
	// Read time from CMOS
	cmos_readtime();

	urm_create_dir("/system", "time");

	printk("Time:year:%d month:%d day:%d week:%d hour:%d min:%d sec:%d\n", \
	ktime.year, ktime.month, ktime.day, ktime.week_day, ktime.hour, ktime.mintus, ktime.second);
}
