
sinclude $(CURDIR)/../../Makefile.variable

DIRS := $(shell find . -maxdepth 9 -type d)

C_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
CPP_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
ASM_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.asm))

OBJS := $(patsubst %.c, %.o, $(C_SRCS))
OBJS += $(patsubst %.cpp, %.o, $(CPP_SRCS))
OBJS += $(patsubst %.asm, %.o, $(ASM_SRCS))

ARFLAGS  := -rs
CFLAGS   := -m32 -fno-stack-protector -I $(CURDIR)/Includes
CPPFLAGS := -m32 -fno-stack-protector -I $(CURDIR)/Includes
ASMFLAGS := -f elf

sinclude $(OBJS:.o=.d)

TARGET = libappbase.a

appbase: $(TARGET)

$(TARGET): $(OBJS)
	@$(AR) $(ARFLAGS) $(TARGET) $(OBJS)

all:
	@echo Making appbase...
	@make appbase

%.d: %.asm
	@$(NS) -M $(ASMFLAGS) $< > $@; \
	$(SED) -i 's,^:,$*.o $@:,g' $@;

%.d: %.c
	@echo [CC] $<
	@$(CC) -M $(CFLAGS) $< > $@; \
	$(SED) -i 's,$(notdir $*)\.o[ :]*,$*.o $@ : ,g' $@;

%.d: %.cpp
	@echo [CXX] $@
	@$(CXX) -M $(CPPFLAGS) $< > $@; \
	$(SED) -i 's,$(notdir $*)\.o[ :]*,$*.o $@ : ,g' $@;

%.o: %.asm
	@echo [NS] $@
	@$(NS) $(ASMFLAGS) -o $@ $<
