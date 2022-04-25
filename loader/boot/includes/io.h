// Explorer Boot
// /boot/include/io.h
// Defination of I/O functions

#ifndef IO_H_
#define IO_H_

void io_hlt(void);

void io_cli(void);
void io_sti(void);

// Input functions
unsigned char  io_in8 (unsigned short port);
unsigned short io_in16(unsigned short port);
unsigned int   io_in32(unsigned short port);

// Output functions
void io_out8 (unsigned short port, unsigned char  data);
void io_out16(unsigned short port, unsigned short data);
void io_out32(unsigned short port, unsigned int   data);

#endif
