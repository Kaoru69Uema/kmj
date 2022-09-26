TARGET =  usrt

HEDS   =                           # hoge.h bar.h

SRCS   =  $(TARGET).c

OBJS   =  $(SRCS:.c=.o)

AVR_CHIP  = attiny85

AVR_CHIP2 = t85

PORT = /dev/ttyU0

CC = avr-gcc

CFLAGS = -Wall -Os -mmcu=$(AVR_CHIP)

SIZE    = avr-size --totals --radix=16

RM = -rm -f

# Hexファイル生成
$(TARGET).hex: $(OBJS)
	$(CC) $() $(CFLAGS) $(OBJS) -o $(TARGET).elf
	avr-objcopy -F ihex $(TARGET).elf $(TARGET).hex
	$(SIZE) $(TARGET).elf

%.o: %.c $(HEDS)
	$(CC) $(CFLAGS) -c $< -o $@

upload: $(TARGET).hex
	avrdude -c usbasp -p $(AVR_CHIP2) -P $(PORT) \
                     -U flash:w:$(TARGET).hex:i

bupload: $(TARGET).hex
	doas avrdude -c usbasp -p $(AVR_CHIP2) -P $(PORT) \
                     -U flash:w:$(TARGET).hex:i

edit:
	vim $(SRCS)

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) $(TARGET).elf $(TARGET).hex
