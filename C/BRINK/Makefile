# Makefile : ひな型

TARGET =  Blink
HEDS   =  #AvrInit.h PBToggle.h
SRCS   =  blink.c
OBJS   =  $(SRCS:.c=.o)     # AvrInit.o PBToggle.o main.o
# AVR デバイス情報
AVR_CHIP = attiny85
# コンパイラ情報
CC = avr-gcc
CFLAGS = -Wall -Os -mmcu=$(AVR_CHIP)
# その他
SIZE    = avr-size --totals --radix=16  # --mcu=$(AVR_CHIP)
RM = -rm -f
# Hexファイル生成
$(TARGET).hex: $(OBJS)
	$(CC) $() $(CFLAGS) $(OBJS) -o $(TARGET).elf
	avr-objcopy -F ihex $(TARGET).elf $(TARGET).hex
	$(SIZE) $(TARGET).elf
# 各ソースのオブジェクトファイル生成
%.o: %.c $(HEDS)
	$(CC) $(CFLAGS) -c $< -o $@

upload: $(TARGET).hex
	doas avrdude -c usbasp -p t85 -P /dev/ttyU0 \
                     -U flash:w:$(TARGET).hex:i
edit:
	vim $(SRCS)

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) $(TARGET).elf $(TARGET).hex
