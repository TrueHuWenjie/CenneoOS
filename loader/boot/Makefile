# Date:Miss
# Author:HuWenjie<huwenjie@cenneo.com>
# Explorer Loader
# makefile for Explorer Loader Boot
#
# BSD 2-Clause License
#
# Copyright (c) 2017, Ghost Bird Operating System Project Developers.
# All rights reserved.

# Note that there is a boota.asm, which is boota.o in the first element
# of $(OBJ), because it has entry "_start"

OBJS := ./arch/x86/_start.o ./arch/x86/io.o ./arch/x86/halt.o \
./arch/x86/interrupt/handle.o ./arch/x86/interrupt/entry.o \
./arch/x86/interrupt/trap.o ./boota.o ./MMU.o ./bootc.o ./VI.o ./fs.o \
./storage.o ./elf.o ./lib/string.o ./lib/font.o ./lib/vsprintf.o ./lib/mem.o \
./script.o ./lib/graphics.o ./lib/math.o

OBJCOPYFLAGS := -R .note -R .comment -S

LDFLAGS  := -m elf_i386 -e _start
CFLAGS   := -m32 -fno-stack-protector -I $(CURDIR)/include
ASMFLAGS := -f elf

TARGET = BOOT

sinclude $(OBJS:.o=.d)

install:BOOT
	$(GFR) -ws $(TARGET) $(image) 2

all: $(TARGET)

# compile files

$(TARGET):setup.bin boot.bin
	cat setup.bin boot.bin > $(TARGET)

boot.bin:boot.o
	$(OBJCOPY) $(OBJCOPYFLAGS) -O binary boot.o boot.bin

boot.o:$(OBJS)
	$(LD) $(LDFLAGS) -o $@ -Ttext 0x11000 $(OBJS)

setup.bin:setup.basm
	nasm setup.basm -o setup.bin

%.o: %.asm
	$(NS) $(ASMFLAGS) -o $@ $<

%.d: %.asm
	@$(NS) -M $(ASMFLAGS) $< > $@; \
	sed -i 's,^:,$*.o $@:,g' $@;

%.d: %.c
	@$(CC) -M $(CFLAGS) $< > $@; \
	sed -i 's,$(notdir $*)\.o[ :]*,$*.o $@ : ,g' $@;

clean:
	$(RM) BOOT $(OBJS) $(OBJS:.o=.d) boot.o *.bin
