// Cenneo OS
// /kernel/kio/kio.c
// Kernel Log System

#include <types.h>
#include <stdarg.h>
#include <stdio.h>

struct kls_callback_chain_def
{
    struct kls_callback_chain_def *next;

} kls_callback_chain;

#define KVI_BUF_SIZE 256

/**printk函数，格式化输出字符串到屏幕上*/
int printk(const char *fmt, ...)
{
    char buffer[KVI_BUF_SIZE];
    va_list arg;
    unsigned long n;

	/**停止调度*/
	// disable_schedule();

	/**format*/
	va_start(arg, fmt);/*init argument point*/
	vsprintf(buffer, fmt, arg);/*write format & get width*/

	/**循环将字写入屏幕*/
	for (n = 0; buffer[n] != 0; n ++) kvi_put_char(buffer[n]);

	/**防止空指针引发危险*/
	va_end(arg);

	/**允许调度*/
	// enable_schedule();

	/**正常返回*/
	return n;
}

int fault(void)
{

}

int warn(const char *fmt, ...)
{
    char buffer[KVI_BUF_SIZE];
    va_list arg;
    unsigned long n;

    /**停止调度*/
    // disable_schedule();

    //kvi_color(KVI_WARN_FGCOLOR, KVI_WARN_BGCOLOR);

    /**format*/
    va_start(arg, fmt);/*init argument point*/
    vsprintf(buffer, fmt, arg);/*write format & get width*/

    /**循环将字写入屏幕*/
    for (n = 0; buffer[n] != 0; n ++) kvi_put_char(buffer[n]);

    /**防止空指针引发危险*/
    va_end(arg);

    /**允许调度*/
    // enable_schedule();

    /**正常返回*/
    return n;
}

int error(const char *fmt, ...)
{
    char buffer[KVI_BUF_SIZE];
    va_list arg;
    unsigned long n;

    /**停止调度*/
    // disable_schedule();

    kvi_disable();

    //kvi_color(KVI_EROR_FGCOLOR, KVI_EROR_BGCOLOR);

    /**format*/
    va_start(arg, fmt);/*init argument point*/
    vsprintf(buffer, fmt, arg);/*write format & get width*/

    /**循环将字写入屏幕*/
    for (n = 0; buffer[n] != 0; n ++) kvi_put_char(buffer[n]);

    /**防止空指针引发危险*/
    va_end(arg);

    /**允许调度*/
    // enable_schedule();

    while (1) io_hlt();

    /**正常返回*/
    return n;
}

void kls_set_intercept()
{

}

// Initialization of Kernel Log System
void init_kls(void)
{

}
