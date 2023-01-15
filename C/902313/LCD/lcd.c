#include "lcd.h"

//const unsigned char a[] PROGMEM = "1996/06/30 Sachiko & Tsuyoshi Born! Good Good Good!!!";


void port_init( void )
{
           LCD_DIR = (0xf0) + (1<<LCD_E) + (1<<LCD_RS);         // LCD_DATA_(7,6,5,4),  LCD_E,  LCD_RS  .
           LCD     = ( 1<<LCD_RS );                              // HIGH==データ（デフォルト）。LOW==コマンド
           DDRD    = ( 1<<PD6 );                                  // TEST_LED( PD6 ) .
}


void lcd_init( void )
{
           _delay_ms( 500 );
           lcd_cmd_4bit( 0x30 );
           _delay_ms(5);
           lcd_cmd_4bit( 0x30 );
           _delay_ms(5);
           lcd_cmd_4bit( 0x30 );
           _delay_ms(1);
           lcd_cmd_4bit( 0x20 );
           lcd_cmd_8bit( 0x28 );
           lcd_cmd_8bit( 0x0e );
           lcd_cmd_8bit( 0x06 );
           lcd_cmd_8bit( 0x01 );
           _delay_ms( 15 );
}


void lcd_put_4bit( unsigned char d )
{
           LCD = LCD & 0x0F;
           LCD = LCD | ( d & 0xf0 );
           LCD |=  (1<<LCD_E);             // HIGH
           _delay_us( 20 );
           LCD &= ~(1<<LCD_E);             // LOW
           _delay_us( 20 );
}


void lcd_put_8bit( unsigned char d )
{
           lcd_put_4bit( d & 0xf0 );
           lcd_put_4bit( d << 4 );
//           _delay_ms( 10 );
}

void lcd_cmd_4bit( unsigned char c )
{
           LCD &= ~(1<<LCD_RS);            // LOW
           lcd_put_4bit( c );
           LCD |=  (1<<LCD_RS);            // HIGH
}

void lcd_cmd_8bit( unsigned char c )
{
           LCD &= ~(1<<LCD_RS);            // LOW
           lcd_put_8bit( c );
           LCD |=  (1<<LCD_RS);            // HIGH
}

