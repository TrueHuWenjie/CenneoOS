/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Intel 8042 chip support
 * Explorer/drivers/i8042.c
 * version:Alpha
 * 11/29/2014 12:52 AM
 */

#include <arch.h>
#include "i8042.h"

#define PORT_8042_DATA		0x60
#define PORT_8042_COMMAND	0x64

// Wait for Intel 8042
void i8042_wait(void)
{
	while (1)
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0)
			break;
			
	return;
}

// Send command to Intel 8042
void i8042_cmd(char command)
{
	i8042_wait();
	io_out8(PORT_8042_COMMAND, command);
}

// Read data from Intel 8042
char i8042_read(void)
{
	i8042_wait();
	return io_in8(PORT_8042_DATA);
}

// Write to Intel 8042
void i8042_write(char b)
{
	i8042_wait();
	io_out8(PORT_8042_DATA, b);
}

// Read command byte
char i8042_read_cmd_byte(void)
{
	i8042_cmd(0x20);
	return i8042_read();
}

// Write command byte
void i8042_write_cmd_byte(char b)
{
	i8042_cmd(0x60);
	i8042_write(b);
	return ;
}

// Send command to keyboard
void i8042_keyboard_cmd(char command)
{
	i8042_write(command);
	i8042_wait();
}

// Send command to mouse
void i8042_mouse_cmd(char command)
{
	i8042_cmd(0xD4);/**8042 knows that the command is send for mouse*/
	i8042_write(command);
}

// Enable mouse
void i8042_mouse_enable(void)
{
	char b;
	b = i8042_read_cmd_byte();
	b = (b & 0b11011111);
	b = (b | 0b10);
	i8042_write_cmd_byte(b);
	i8042_cmd(0xA8);
}

// Disable Mouse
void i8042_mouse_disable(void)
{
	i8042_write_cmd_byte(i8042_read_cmd_byte() | 0b100000);
}

// Enable Keyboard
void i8042_keyboard_enable(void)
{
	i8042_write_cmd_byte(i8042_read_cmd_byte() & 0b11101111);
}
