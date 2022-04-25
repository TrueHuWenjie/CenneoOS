# Explorer Loader
# /mbr/Makefile
# makefile for Explorer Loader MBR

# compile all the MBR

sinclude $(CURDIR)/../../Makefile.variable

TARGET = mbr.bin


$(TARGET): $(TARGET:.bin=.asm)
	@echo [NS] $<
	@$(NS) $< -o $@
	
$(image): $(TARGET)
	@$(GFR) -ws $(TARGET) $(image) 1

all:
	@make $(TARGET)
	
install:$(image)
	
	
clean:
	@$(RM) $(TARGET)
	
