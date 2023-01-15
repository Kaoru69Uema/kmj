#ifndef _LCD_
#define _LCD_
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

//extern const unsigned char a[] PROGMEM;

#define    LCD        PORTB
#define    LCD_DIR    DDRB
#define    LCD_E      PB1
#define    LCD_RS     PB0
#define    LCD_COL    16             // 20 or 16

int  main( void );
void port_init( void );
void lcd_init( void );
void lcd_put_4bit( unsigned char );
void lcd_put_8bit( unsigned char ); // put char
void lcd_cmd_4bit( unsigned char );
void lcd_cmd_8bit( unsigned char );

#endif
