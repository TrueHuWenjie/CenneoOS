// Cenneo OS
// /kernel/Log/kls.c
// Kernel Log System

#include <types.h>
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <lib/string.h>
#include <kls.h>

// The size of kls's buffer
#define KLS_BUF_SIZE 2048
static char *kls_buf = NULL;
static unsigned int kls_buf_offset = 0;

struct kls_log_header
{
    unsigned short length;
    unsigned char class;
    unsigned char level;
    unsigned long timestamp;
};

struct kls_callback_chain_def
{
    struct kls_callback_chain_def *next;
    void (*function)(const char level, const char *log);
} *kls_callback_chain;

#define KVI_BUF_SIZE 256

static void kls_call_functions(const char level, const char *log)
{
    struct kls_callback_chain_def *chain_ptr = kls_callback_chain;

    while (chain_ptr)
    {
        chain_ptr->function(level, log);
        chain_ptr = chain_ptr->next;
    }
}


/**printk函数，格式化输出字符串到屏幕上*/
int printk(const char *fmt, ...)
{
    char buffer[KVI_BUF_SIZE];
    va_list arg;
    unsigned long n;

	// Stop Task Schedule
	// disable_schedule();

	/**format*/
	va_start(arg, fmt);/*init argument point*/
	vsprintf(buffer, fmt, arg);/*write format & get width*/

    // Call the callback chain to update kernel's log on screen
    // or other kind of output device
    kls_call_functions(KLS_LOGLEVEL_NOTE, buffer);

	/**循环将字写入屏幕*/
	for (n = 0; buffer[n] != 0; n ++) kvi_put_char(buffer[n]);

	/**防止空指针引发危险*/
	va_end(arg);

	/**允许调度*/
	// enable_schedule();

	return n;
}

int debug(const char *fmt, ...)
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

int fault(const char *fmt, ...)
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
    disable_schedule();

    //kvi_disable();

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

// Set a intercept to get new log info
void kls_set_intercept(void (*function)(const char level, const char *log))
{
    struct kls_callback_chain_def *listener = NULL;
    while (!listener)
        listener = kmalloc(sizeof(struct kls_callback_chain_def));

    listener->function = function;

    // Insert into chain
    if (kls_callback_chain)
    {
        listener->next = kls_callback_chain;
        kls_callback_chain = listener;
    }else{
        kls_callback_chain = listener;
        listener->next = NULL;
    }
}

// Initialization of Kernel Log System
void init_kls(void)
{
    // Allocate a buffer for temporary store Kernel-Log info
    while (!kls_buf)
        kls_buf = kmalloc(KLS_BUF_SIZE);
    
    memset(kls_buf, 0, KLS_BUF_SIZE);

    kls_callback_chain = NULL;

    urm_create("/kernel/", "log");

    urm_scheme_register("record");
}
