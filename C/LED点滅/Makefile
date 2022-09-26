F_CPU  = 1000000
TARGET =  main
HEDS   =  #AvrInit.h PBToggle.h
SRCS   =  $(TARGET).c       # usart.c eeprom.c
OBJS   =  $(SRCS:.c=.o)     # AvrInit.o PBToggle.o main.o
# AVR デバイス情報
AVR_CHIP  = atmega328p

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

# 自作　HIDaspx　日本製AVRプログラマー使用 
upload: $(TARGET).hex
	hidspx $(TARGET).hex

edit: $(TARGET).c
	vim $(TARGET).c

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) $(TARGET).elf $(TARGET).hex
