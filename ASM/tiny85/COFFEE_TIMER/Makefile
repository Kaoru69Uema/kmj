TARGET = main
MIKON  = t85

all: $(TARGET).hex

$(TARGET).hex: $(TARGET).asm
	avra  $(TARGET).asm

upload: $(TARGET).hex
	hidspx $(TARGET).hex


edit: $(TARGET).asm
	touch $(TARGET).asm
	vim $(TARGET).asm

.PHONY: clean
clean:
	-rm -f $(TARGET).hex $(TARGET).eep.hex $(TARGET).cof $(TARGET).obj
