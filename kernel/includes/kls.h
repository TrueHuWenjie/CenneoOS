// Cenneo OS
// /kernel/include/kls.h
// Kernel Log System

#ifndef KLS_H_

// Log Level
#define KLS_LOGLEVEL_ERROR 0xff
#define KLS_LOGLEVEL_FAULT 0xc0
#define KLS_LOGLEVEL_WARN  0x80
#define KLS_LOGLEVEL_NOTE  0x40
#define KLS_LOGLEVEL_DEBUG 0x00

/**printk函数，格式化输出字符串到屏幕上*/
int printk(const char *fmt, ...);

int fault(const char *fmt, ...);

int warn(const char *fmt, ...);

int error(const char *fmt, ...);

// Set a intercept to get new log info
void kls_set_intercept(void (*function)(const char level, const char *log));

// Initialization of Kernel Log System
void init_kls(void);

#endif
