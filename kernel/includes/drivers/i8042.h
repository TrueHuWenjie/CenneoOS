/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by HuWenjie<huwenjie@cenneo.com>
 * Intel 8042 chip support definition
 * Explorer/drivers/i8042.h
 * version:Alpha
 * 11/29/2014 1:07 AM
 */

#ifndef I8042_H_
#define I8042_H_

//#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47
	
// Waiting until Output Buffer empty
void i8042_wait_obuf_full(void);

// Waiting until Input Buffer empty
void i8042_wait_ibuf_empty(void);

// Write data to Input Buffer as command
void i8042_controller_send_cmd(char command);

// Send command to keyboard
void i8042_keyboard_send_cmd(char command);

// Send command to mouse
void i8042_mouse_send_cmd(char command);

// Read data from Output Buffer
unsigned char i8042_read_obuf(void);

// Read Status Register
unsigned char i8042_read_status(void);

// Enable mouse
void i8042_mouse_enable(void);

// Enable Keyboard
void i8042_keyboard_enable(void);

// Disable Mouse
void i8042_mouse_disable(void);

// Disable Keyboard
void i8042_keyboard_disable(void);

// Initialization of PS/2 Controller
void init_i8042(void);

#endif