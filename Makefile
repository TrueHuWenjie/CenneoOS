# Date:2017/6/20 16:10
# Author:HuWenjie<huwenjie@cenneo.com>
# Cenneo OS
# Makefile
#
# BSD 2-Clause License
#
# Copyright (c) 2017, Ghost Bird Operating System Project Developers.
# All rights reserved.

sinclude $(CURDIR)/Makefile.variable

.PHONY:gfr
.PHONY:loader kernel
.PHONY:$(image)
.PHONY:help
.PHONY:clean
.PHONY:all install run


run:install
	@echo Starting Virtual Machine...
	@$(qemu) -net nic,model=rtl8139 -net user -drive file=$(image),format=vpc -m 9

# tools

gfr:
	@cd tools/gfr && make all

clean_gfr:
	@cd tools/gfr && make clean

# loader

loader:
	@cd loader && make all

clean_loader:
	@cd loader && make clean

# kernel

kernel:
	@cd kernel && make all

clean_kernel:
	@cd kernel && make clean

# appbase

appbase:
	@cd tools/appbase && make all

# applications

console:
	@cd applications/Console && make all

clean_console:
	@cd applications/Console && make clean

gui:
	@cd applications/GUI && make all

prepare:$(image:.vhd = .zip) $(image)
	$(depress) $(image:.vhd=.zip)

clean:
	@echo Cleaning temporary files...
	@cd loader && make clean
	@cd tools/gfr && make clean
	@cd kernel && make clean

dist:clean
	@echo Compressing Image...
	@$(compress) -m -r -j $(image:.vhd=.zip) $(image)

all:loader kernel

sync:
	@echo Synchronizing...

ifeq ($(PLATFORM), LINUX)
	@modprobe nbd
	@qemu-nbd -c /dev/nbd0 $(image) -f vpc
	@sleep 0.2
	@mount /dev/nbd0p1 /mnt
	@rsync -rtopgDl $(CURDIR)/image/data/ /mnt/ --delete
	@umount /mnt
	@qemu-nbd -d /dev/nbd0
	@sleep 0.2
	@rmmod nbd
endif

ifeq ($(PLATFORM), OSX)
	@hdiutil attach -imagekey diskimage-class=CRawDiskImage $(image)
	@rsync -rtopgDl $(CURDIR)/image/data/ /Volumes/CENNEOOS/ --delete
	@sudo umount /Volumes/CENNEOOS
#	@hdiutil detach /dev/disk3
endif

install:all
	@make sync

install_loader:
	@echo Installing Loader...
	@cd loader && make install
	
#$(qemu) -net nic,model=rtl8139 -drive file=$(image),format=vpc -m 9
# $(qemu) -device virtio-net-pci,mac=11:22:33:44:55:66 -drive file=$(image),format=vpc -m 9
