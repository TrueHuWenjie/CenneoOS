// Cenneo OS
// /kernel/arch/x86/power.c
// Power management

#include "include/io.h"

// Reset your computer
void reset(void)
{
	io_out8(0x64,0xfe);
}

// Poweroff
void poweroff(void)
{
	// In Qemu
	io_out16(0x604, 0x2000);

	// In VirtualBox
	io_out16(0x4004, 0x3400);
}
