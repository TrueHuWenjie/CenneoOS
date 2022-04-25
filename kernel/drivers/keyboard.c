// Cenneo OS
// /kernel/Drivers/Keyboard.c
// Keyboard Support

#include <drivers/Keyboard.h>
#include <arch.h>
#include <stdbool.h>
#include <drivers/i8042.h>

// Size of buffer
#define CMD_QUEUE_SIZE		32
#define SCANCODE_QUEUE_SIZE	128

static unsigned char cmd_queue[CMD_QUEUE_SIZE];
static unsigned char scancode_queue[SCANCODE_QUEUE_SIZE];
static unsigned char cmd_untreated;
static unsigned char scancode_untreated;
static unsigned char sc_wi;
static unsigned char sc_ri;
static unsigned char *write_point;
static unsigned char *read_point;

static bool capslock, left_shift, right_shift, left_ctrl, left_alt;

static unsigned char key_value;

/**转换数组*/
static unsigned char key_table_normal[0x7f] = {
	0x00,
	0x1b, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	0x09, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\r',
	0x00, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	0x00, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0x00,
	'*', 0x00, ' ', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0X00, 0X00,
	'7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '_', 0X00, 0X00, 
	0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0x5C, 0X00, 0x00, 0X00
};

static unsigned char key_table_shift[0x7f] = {
	0x00,
	0x1b, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\r',
	0x00, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
	0x00, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0x00,
	'*', 0x00, ' ', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0X00, 0X00,
	'7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '_', 0X00, 0X00, 
	0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0x5C, 0X00, 0x00, 0X00
};

static unsigned char key_table_capslock[0x7f] = {
	0x00,
	0x1b, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	0x09, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\r',
	0x00, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',
	0x00, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0x00,
	'*', 0x00, ' ', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0X00, 0X00,
	'7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '_', 0X00, 0X00, 
	0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0x5C, 0X00, 0x00, 0X00
};

static void keyboard_cmd(unsigned char command)
{
	while (scancode_untreated >= CMD_QUEUE_SIZE);
	//	delay(1);

	scancode_queue[sc_wi] = command;
	if (!scancode_untreated)
	{
		i8042_keyboard_cmd(command);
	}else{

	}
}

static unsigned char get_scancode(void)
{
	unsigned char code;
	
	for (; !scancode_untreated;) io_hlt();

	// Fetching code from FIFO
	code = *read_point;
	if (read_point != (scancode_queue + SCANCODE_QUEUE_SIZE - 1))
	{
		read_point ++;
	}else{
		read_point = scancode_queue;
	}
	scancode_untreated --;

	return code;
}

/**
 * This function made by Lab Explorer Developers<1@GhostBirdOS.org>
 * return keyboard scan code if there is a code in buffer.
 * wait if no scan code.
 */

unsigned char get_key(void)
{
	unsigned char code;

	code = get_scancode();

	// Converting Code to ASCII
	if ((code != 0x00) && (code < 0x80))
	{
		if (left_shift || right_shift)
		{
			return key_table_shift[code];
		}else if (capslock)
		{
			return key_table_capslock[code];
		}else{
			return key_table_normal[code];
		}
	}

	return 0;
}

/**
 * This function made by Lab Explorer Developers<1@GhostBirdOS.org>
 * This function run when a interrupt of keyboard happen,
 * first,we get scan code of keyboard from port 0x60 and
 * save it.
 * Then, we try to move the code into the buffer of keyboard
 * if there is a space for the code(untreated != SCANCODE_QUEUE_SIZE)
 * after that,the write_point need to point to next space.
 * 
 * in the last,we need to set key flag if the scan code is 
 * a function key.
 */

void int_keyboard_handle(void)
{

	key_value = i8042_read_obuf();
printk("key_val:%#x\n", key_value);
	/**compare if it is a functional key*/
	switch (key_value)
	{
		// Left Shift
		case 0x2A:
			left_shift = true;
			break;
		case 0xAA:
			left_shift = false;
			break;

		// Right Shift
		case 0x36:
			right_shift = true;
			break;
		case 0xB6:
			right_shift = false;
			break;

		// Left Ctrl
		case 0x1D:
			left_ctrl = true;
			break;
		case 0x9D:
			left_ctrl = false;
			break;

		// Left Alt
		case 0x38:
			left_alt = true;
			break;
		case 0xB8:
			left_alt = false;
			break;

		default:
			if (scancode_untreated != SCANCODE_QUEUE_SIZE)
			{
				*write_point = key_value;
				if (write_point != (scancode_queue + SCANCODE_QUEUE_SIZE - 1))
				{
					write_point ++;
				}else{
					write_point = scancode_queue;
				}
				scancode_untreated ++;
			}
			break;
	}
	
	
	i8259_eoi();
}

void init_keyboard(void)
{
	cmd_untreated = 0;
	scancode_untreated = 0;
	sc_wi = 0;
	sc_ri = 0;
	write_point = scancode_queue;
	read_point = scancode_queue;
	
	/*Initialize key flag*/
	left_shift = false;
	right_shift = false;
	left_ctrl = false;
	left_alt = false;
	capslock = false;

	register_PIC(1, &int_keyboard_handle, "keyboard");

	// Reset Keyboard
	//i8042_keyboard_cmd(0xff);
	
	// ACK
	//i8042_read_obuf();
	
	//if (i8042_read_obuf() != 0xaa)
	//{
	//	printk("Keyboard initialization failed.\n");
	//	return;
	//}

	// Set LED
	//i8042_keyboard_cmd(0xED);
	
	// All LED turn off
	//i8042_keyboard_cmd(0x00);

	return;
}
