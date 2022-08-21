# 変数宣言
## ソース情報
TARGET = Blink
HEDS = AvrInit.h PBToggle.h
SRCS = AvrInit.c PBToggle.c main.c
OBJS = $(SRCS:.c=.o) # AvrInit.o PBToggle.o main.o
## AVR デバイス情報
AVR_CHIP = atmega328p
AVR_WRIT = arduino
AVR_PORT = /dev/ttyACM0
## コンパイラ情報
CC = avr-gcc
CFLAGS = -Wall -Os -mmcu=$(AVR_CHIP)
## 他
RM = -rm

# 実行ファイル及び HEX ファイル作成 (各 Cソースのオブジェクトファイルが必要)
$(TARGET).hex: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	avr-objcopy -F ihex $(TARGET) $(TARGET).hex

# 各 Cソースのオブジェクトファイル作成
%.o: %.c $(HEDS)
	$(CC) $(CFLAGS) -c $< -o $@

# HEX ファイル書き込み (Blink.hex が必要)
write: $(TARGET).hex
	avrdude -c $(AVR_WRIT) -P $(AVR_PORT) -p $(AVR_CHIP) -U flash:w:$(TARGET).hex:i

# オブジェクトファイルと実行ファイル及び HEX ファイル削除
.PHONY:clean
clean:
	$(RM) *.o
	$(RM) $(TARGET) $(TARGET).hex
