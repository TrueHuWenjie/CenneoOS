# Date:2017/6/20 16:10
# Author:HuWenjie<1@hwj.me>
# Ghost Bird OS
# Makefile
# 
# BSD 2-Clause License
# 
# Copyright (c) 2017, Ghost Bird Operating System Project Developers.
# All rights reserved.

CC      = gcc
CXX     = g++
NS      = nasm
LD      = ld
OBJCOPY = objcopy
GFR     = gfr
qemu = qemu-system-i386
image = $(CURDIR)/image/image.vhd
compress = zip
depress = unzip

export CC NS LD OBJCOPY GFR
export image qemu

export PATH += :$(CURDIR)/tools/gfr

.PHONY:gfr
.PHONY:loader kernel
.PHONY:$(image)
.PHONY:help
.PHONY:clean
.PHONY:all install run

gfr:
	cd tools/gfr && make all
	
loader:
	cd loader && make all

kernel:
	cd kernel && make all

prepare:$(image:.vhd = .zip) $(image)
	$(depress) $(image:.vhd=.zip) -d image/ 

clean_loader:
	cd loader && make clean

clean_gfr:
	cd tools/gfr && make clean

clean_kernel:
	cd kernel && make clean

clean:
	cd loader && make clean
	cd tools/gfr && make clean
	cd kernel && make clean

dist:clean
	$(compress) -m -r -j $(image:.vhd=.zip) $(image)
	
install:
	cd loader && make install
	
all:
	cd loader && make all
	
run:all install
	$(qemu) -drive file=$(image),format=vpc

help:
	clear
	@echo Ghost Bird Project Makefile - Help
	@echo target		introduction
	@echo gfr		make Ghost Bird File System Reader
	@echo loader		make Explorer Loader
	@echo clean		make cl
	@echo loader		make Explorer Loader

