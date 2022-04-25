/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * stdio.h
 * Explorer/include/stdio.h
 * 2/5/2015 3:2 PM
 */

#ifndef STDIO_H_
#define STDIO_H_

#include <stdarg.h>

#define BUF_SIZE 512

int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);
int printf(const char *format, ...);

#endif
