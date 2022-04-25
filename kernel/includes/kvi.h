// Cenneo OS
// /kernel/include/kvi.h
// Kernel-support visual interface

#ifndef KVI_H_
#define KVI_H_

#include <types.h>

// Clear the Screen
void kvi_clear(void);

void kvi_put_char(char ascii);
void kvi_roll_screen(void);
void kvi_color(u32 fg_color, u32 bg_color);

int printk(const char *fmt, ...);
int warn(const char *fmt, ...);
int error(const char *fmt, ...);

//Open kvi
void kvi_open(void);

//Close kvi
void kvi_close(void);

#endif
