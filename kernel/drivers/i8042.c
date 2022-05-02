/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Intel 8042 chip support
 * Explorer/drivers/i8042.c
 * version:Alpha
 * 11/29/2014 12:52 AM
 */

#include <arch.h>
#include <drivers/i8042.h>

#define I8042_PORT_DATA		0x60
#define I8042_PORT_CMD		0x64

#define I8042_REG_STATUS_OBUF_FULL		0x01
#define I8042_REG_STATUS_IBUF_FULL		0x02
#define I8042_REG_STATUS_SYS_FLAG		0x04
#define I8042_REG_STATUS_CMD_DATA		0x08
#define I8042_REG_STATUS_KYBD_INH		0x10
#define I8042_REG_STATUS_TRANS_TMOUT	0x20
#define I8042_REG_STATUS_RCV_TMOUT		0x40
#define I8042_REG_STATUS_PARITY_EVEN	0x80

// Read Status Register
unsigned char i8042_read_status(void)
{
	return io_in8(I8042_PORT_CMD);
}

// Waiting until Output Buffer empty
void i8042_wait_obuf_full(void)
{
	while (1)
		if ((i8042_read_status() & I8042_REG_STATUS_OBUF_FULL) == 1)
			return;
}

// Waiting until Input Buffer empty
void i8042_wait_ibuf_empty(void)
{
	while (1)
		if ((i8042_read_status() & I8042_REG_STATUS_IBUF_FULL) == 0)
			return;
}

// Read data from Output Buffer
unsigned char i8042_read_obuf(void)
{
	i8042_wait_obuf_full();
	return io_in8(I8042_PORT_DATA);
}

// Write command to PS/2 Controller
void i8042_controller_send_cmd(char command)
{
	i8042_wait_ibuf_empty();
	io_out8(I8042_PORT_CMD, command);
}

// Write data to PS/2 Controller
void i8042_controller_send_data(char data)
{
	i8042_wait_ibuf_empty();
	io_out8(I8042_PORT_DATA, data);
}

// Send command to keyboard
void i8042_keyboard_send_cmd(char command)
{
	i8042_wait_ibuf_empty();
	io_out8(I8042_PORT_DATA, command);
	i8042_wait_ibuf_empty();
}

// Send command to mouse
void i8042_mouse_send_cmd(char command)
{
	i8042_controller_send_cmd(0xD4);/**8042 knows that the command is send for mouse*/
	//i8042_wait_ibuf_empty();
	io_out8(I8042_PORT_DATA, command);
}

// Initialization of PS/2 Controller
void init_i8042(void)
{
	unsigned char cfg_byte;

	// Check if PS/2 controller exists

	// Disable Devices
	i8042_controller_send_cmd(0xa7);
	i8042_controller_send_cmd(0xad);

	// Flush the output buffer
	while (1)
		if ((i8042_read_status() & I8042_REG_STATUS_OBUF_FULL) == 1)
			printk("Flush:%#x ", io_in8(I8042_PORT_DATA));
		else
			break;

	// Get Configuration Byte
	i8042_controller_send_cmd(0x20);
	cfg_byte = i8042_read_obuf();

	// Self Test
	i8042_controller_send_cmd(0xaa);
	if (i8042_read_obuf() != 0x55)
	{
		printk("PS/2: Intel 8042 self test failed.\n");
		return;
	}

	// Keyboard
	init_keyboard();
	
	// Mouse
	init_mouse();

	cfg_byte |= 0x01;
	cfg_byte |= 0x02;

	i8042_controller_send_cmd(0x60);
	i8042_controller_send_data(cfg_byte);

	// Enable keyboard and mouse
	i8042_controller_send_cmd(0xae);
	i8042_controller_send_cmd(0xa8);
}
