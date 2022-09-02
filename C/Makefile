# Makefile  ひな型
F_CPU  = 8000000
TARGET =  blink
HEDS   =  #AvrInit.h PBToggle.h
SRCS   =  $(TARGET).c       # usart.c eeprom.c
OBJS   =  $(SRCS:.c=.o)     # AvrInit.o PBToggle.o main.o
# AVR デバイス情報
AVR_CHIP  = attiny85
AVR_CHIP2 = t85
# ポート
PORT = /dev/ttyU0

# コンパイラ情報
CC = avr-gcc
CFLAGS = -Wall -Os -mmcu=$(AVR_CHIP) -DF_CPU=$(F_CPU)
# その他
SIZE    = avr-size --format=avr --mcu=$(AVR_CHIP)
RM = -rm -f
# Hexファイル生成
$(TARGET).hex: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET).elf
	avr-objcopy -F ihex $(TARGET).elf $(TARGET).hex
	$(SIZE) $(TARGET).elf
# 各ソースのオブジェクトファイル生成
%.o: %.c $(HEDS)
	$(CC) $(CFLAGS) -c $< -o $@

write: $(TARGET).hex
	doas avrdude -c $(PROGRAMMER) \
                     -p $(AVR_CHIP2) \
                     -P $(PORT) \
                     -U flash:w:$(TARGET).hex:i

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) $(TARGET).elf $(TARGET).hex
