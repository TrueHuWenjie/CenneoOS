
sinclude $(CURDIR)/../../Makefile.variable

DIRS := $(shell find . -maxdepth 9 -type d)

C_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
CPP_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))

OBJS := $(patsubst %.c, %.o, $(C_SRCS))
OBJS += $(patsubst %.cpp, %.o, $(CPP_SRCS))

LDFLAGS  := -m elf_i386 -Ttext-segment 0x80048000
CFLAGS   := -m32 -fno-stack-protector -I $(CURDIR)/includes -I $(appbase)/includes
CPPFLAGS := -m32 -fno-stack-protector -I $(CURDIR)/includes -I $(appbase)/includes

TARGET = ../../image/data/CONSOLE.O

sinclude $(OBJS:.o=.d)

all: $(TARGET) appbase

appbase:
	@cd $(appbase) && make all

$(TARGET): $(OBJS) appbase
	@echo [LD] $@
	@$(LD) $(LDFLAGS) -o $(TARGET) $(OBJS) -static $(appbase)/libappbase.a

%.o: %.c
	@echo [CC] $@
	@$(CC) -c $(CFLAGS) $< -o $@

%.o: %.cpp
	@echo [C++] $@
	@$(CXX) -c $(CPPFLAGS) $< -o $@

%.d: %.c
	@$(CC) -M $(CFLAGS) $< > $@; \
	$(SED) -i 's,$(notdir $*)\.o[ :]*,$*.o $@ : ,g' $@;

%.d: %.cpp
	@$(CXX) -M $(CPPFLAGS) $< > $@; \
	$(SED) -i 's,$(notdir $*)\.o[ :]*,$*.o $@ : ,g' $@;

clean:
	@echo [RM] Removing files...
	@$(RM) $(TARGET) kernel.o *.exe $(OBJS) $(OBJS:.o=.d)
