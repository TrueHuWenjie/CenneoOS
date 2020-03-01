# Explorer Loader
# /mbr/Makefile
# makefile for Explorer Loader MBR

# compile all the MBR

TARGET = MBR.BIN


$(TARGET): $(TARGET:.BIN=.ASM)
	$(NS) $< -o $@
	
all:
	make MBR.BIN
	
install:$(TARGET)
	$(GFR) -ws $(TARGET) $(image) 1
	
clean:
	$(RM) $(TARGET)
	
