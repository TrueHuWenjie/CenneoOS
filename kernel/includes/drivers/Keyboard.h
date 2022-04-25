// Cenneo OS
// /kernel/Includes/drivers/Keyboard.h
// Keyboard Header

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdbool.h>

/**初始化键盘函数*/
void init_keyboard(void);

/**键盘中断处理函数*/
void int_keyboard_handle(void);

// Get new input in ASCII format
unsigned char get_key(void);

#endif