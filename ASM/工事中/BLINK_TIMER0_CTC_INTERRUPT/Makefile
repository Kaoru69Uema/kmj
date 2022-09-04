TARGET = blink
MIKON  = t85
PORT   = /deb/ttyU0

all: $(TARGET).hex

$(TARGET).hex: $(TARGET).asm
	avra -I $(HOME)/AVR/avr-def $(TARGET).asm

up: $(TARGET).hex
	avrdude -c usbasp -p $(MIKON) -P $(PORT) -U flash:w:$(TARGET).hex:i

bup: $(TARGET).hex   # OpenBSD用
	doas avrdude -c usbasp -p $(MIKON) -P $(PORT)  -U flash:w:$(TARGET).hex:i

edit: $(TARGET).asm
	touch $(TARGET).asm
	vim $(TARGET).asm

.PHONY: clean
clean:
	-rm -f $(TARGET).hex $(TARGET).eep.hex $(TARGET).cof $(TARGET).obj
