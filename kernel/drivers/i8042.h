/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Intel 8042 chip support definition
 * Explorer/drivers/i8042.h
 * version:Alpha
 * 11/29/2014 1:07 AM
 */

#ifndef I8042_H_
#define I8042_H_

/**some port of Intel 8042 chip*/
#define PORT_DATA				0x0060
#define PORT_COMMAND			0x0064
#define PORT_KEYSTA				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47
	
// Wait for Intel 8042
void i8042_wait(void);

// Send command to Intel 8042
void i8042_cmd(char command);

// Read data from Intel 8042
char i8042_read(void);

// Write to Intel 8042
void i8042_write(char b);

// Read command byte
char i8042_read_cmd_byte(void);

// Write command byte
void i8042_write_cmd_byte(char b);

// Send command to keyboard
void i8042_keyboard_cmd(char command);

// Send command to mouse
void i8042_mouse_cmd(char command);

// Enable mouse
void i8042_mouse_enable(void);

// Disable Mouse
void i8042_mouse_disable(void);

// Enable Keyboard
void i8042_keyboard_enable(void);

#endif