# Date:2017/6/8 2:21
# Author:HuWenjie<huwenjie@cenneo.com>
# Ghost Bird File System Reader
# Makefile
# 
# BSD 2-Clause License
# 
# Copyright (c) 2017, Ghost Bird Operating System Project Developers.
# All rights reserved.

sinclude $(CURDIR)/../../Makefile.variable

DIRS := $(shell find . -maxdepth 9 -type d)
SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
OBJS := $(patsubst %.c, %.o, $(SRCS))
LDFLAGS := -Wall -O2
CFLAGS := -fno-stack-protector
GFR = gfr

EXEC_NAME = $(GFR)

sinclude $(SRCS:.c=.d)

all: $(EXEC_NAME)


$(EXEC_NAME): $(OBJS)
	$(TOOLS_CC) $(LDFLAGS) -o $(EXEC_NAME) $(OBJS)

%.o: %.c
	@echo [TOOLS_CC] $@
	$(TOOLS_CC) -c $(CFLAGS) $< -o $@

%.d: %.c
	@$(TOOLS_CC) -M $(CFLAGS) $< > $@; \
	$(SED) -i 's,$(notdir $*)\.o[ :]*,$*.o $@ : ,g' $@;

clean:
	$(RM) $(EXEC_NAME) *.exe $(OBJS) $(SRCS:.c=.d)
