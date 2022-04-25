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

sinclude $(CURDIR)/../../Makefile.variable

DIRS := $(shell find . -maxdepth 9 -type d)

C_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
ASM_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.asm))

OBJS := $(patsubst %.c, %.o, $(C_SRCS))
OBJS += $(patsubst %.asm, %.o, $(ASM_SRCS))
OBJS := $(filter %_start.o, $(OBJS)) $(filter-out %_start.o %setup.o, $(OBJS))

OBJCOPYFLAGS := -R .note -R .comment -S

LDFLAGS  := -m elf_i386 -e _start
CFLAGS   := -m32 -fno-stack-protector -I $(CURDIR)/includes
ASMFLAGS := -f elf

TARGET = BOOT

sinclude $(OBJS:.o=.d)

install:$(TARGET)
	$(GFR) -ws $(TARGET) $(image) 2
	
all: $(TARGET)

# compile files

$(TARGET):setup.bin boot.bin
	@echo [CAT] $@
	cat setup.bin boot.bin > $(TARGET)

boot.bin:boot.o
	@echo [OBJCOPY] $@
	@$(OBJCOPY) $(OBJCOPYFLAGS) -O binary boot.o boot.bin

boot.o:$(OBJS)
	@echo [LD] $@
	@$(LD) $(LDFLAGS) -o $@ -Ttext 0x11000 $(OBJS)

setup.bin:setup.asm
	@echo [NS] $@
	@$(NS) -o $@ $<

%.o: %.asm
	@echo [NS] $@
	@$(NS) $(ASMFLAGS) -o $@ $<

%.o: %.c
	@echo [CC] $@
	$(CC) -c $(CFLAGS) $< -o $@

%.d: %.asm
	@$(NS) -M $(ASMFLAGS) $< > $@; \
	$(SED) -i 's,^:,$*.o $@:,g' $@;

%.d: %.c
	@$(CC) -M $(CFLAGS) $< > $@; \
	$(SED) -i 's,$(notdir $*)\.o[ :]*,$*.o $@ : ,g' $@;

clean:
	@echo [RM] Removing files...
	@$(RM) BOOT $(OBJS) $(OBJS:.o=.d) *.bin
