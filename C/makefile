# Makefile : ひな型

TARGET =  Blink
HEDS   =  #AvrInit.h PBToggle.h
SRCS   =  main.c
OBJS   =  $(SRCS:.c=.o)     # AvrInit.o PBToggle.o main.o
# AVR デバイス情報
AVR_CHIP = at90s2313
# コンパイラ情報
CC = avr-gcc
CFLAGS = -Wall -Os -mmcu=$(AVR_CHIP)
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
	hidspx $(TARGET).hex

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) $(TARGET).elf $(TARGET).hex
