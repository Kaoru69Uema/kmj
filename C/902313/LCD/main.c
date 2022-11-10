/***************************************************************************************************/
/* 8TAB                                                                                            */
/*                     --------                      ------------                                  */
/*                    |        |                    |            |                                 */
/*  +5[V]------------ |     PB7|--------LCD_DATA_7--|            |--+5[V]                          */
/*  GND-------------- |     PB6|--------LCD_DATA_6--|    L       |----GND                          */
/*                    |     PB5|--------LCD_DATA_5--|            |                                 */
/*                    |   9 PB4|--------LCD_DATA_4--|    C       |----VO---GND                     */
/*                    |   0    |                    |            |----R/W--GND                     */
/*                    |   S    |                    |    D       |                                 */
/*                    |   2    |                    |            |                                 */
/*                    |   3 PB1| --------LCD_E------|            |----LCD_DATA_3--OPEN             */
/*                    |   1 PB0| --------LCD_RS-----|            |----LCD_DATA_2--OPEN             */
/*                    |   3    |                    |            |----LCD_DATA_1--OPEN             */
/*                    |        |                    |            |----LCD_DATA_0--OPEN             */
/*                    |        |                     ------------                                  */
/*                    |        |                                                                   */
/*                     --------                                                                    */
/*                                                                                                 */
/*                                                                                                 */
/*                                                                                                 */
/*                                                                                                 */
/*                                                                                                 */
/*                      回路図                                                                     */
/*                                                                                                 */
/*                                                                                                 */
/***************************************************************************************************/

/*-------------------------------------------------------------------------------------------------*/
/*                                                                                                 */
/*                                                                                                 */
/*         ヘッダファイル宣言                                                                      */
/*                                                                                                 */
/*                                                                                                 */
/*-------------------------------------------------------------------------------------------------*/

#include "lcd.h"

/*-------------------------------------------------------------------------------------------------*/
/*                                                                                                 */
/*                                                                                                 */
/*         グローバル変数・定数・宣言                                                              */
/*                                                                                                 */
/*                                                                                                 */
/*-------------------------------------------------------------------------------------------------*/

const unsigned char a[] PROGMEM = "Let me try again!";
const unsigned char b[] PROGMEM = "                  ";
const unsigned char c[] PROGMEM = "You and I again.";
const unsigned char d[] PROGMEM = "We made New House!";


/*-------------------------------------------------------------------------------------------------*/
/*                                                                                                 */
/*                                                                                                 */
/*         プロトタイプ宣言                                                                        */
/*                                                                                                 */
/*                                                                                                 */
/*-------------------------------------------------------------------------------------------------*/
int  main( void );

int main( void )
{
           unsigned char temp;
           unsigned char i;
           
           port_init();         // ポート設定
           lcd_init();          // LCD初期化
           
           
           
           while( 1 ) {
                      i=0;
                      lcd_cmd_8bit( 0x80 );
                      
                      while( (temp=pgm_read_byte( &d[i]) ) != '\0' ) {
                                 lcd_put_8bit( temp );
                                 _delay_ms( 260 );
                                 if( i%(LCD_COL*2)==LCD_COL  -1 ) lcd_cmd_8bit( 0x80 + 0x40 );
                                 if( i%(LCD_COL*2)==LCD_COL*2-1 ) lcd_cmd_8bit( 0x01        );
                                 i++;
                      }
                                 _delay_ms( 1000 );
                      i=0;
                      lcd_cmd_8bit( 0x80 );
                      
                      while( (temp=pgm_read_byte( &b[i]) ) != '\0' ) {
                                 lcd_put_8bit( temp );
                                 _delay_ms( 30);
                                 if( i%(LCD_COL*2)==LCD_COL  -1 ) lcd_cmd_8bit( 0x80 + 0x40 );
                                 if( i%(LCD_COL*2)==LCD_COL*2-1 ) lcd_cmd_8bit( 0x01        );
                                 i++;
                      }
                      i=0;
                      lcd_cmd_8bit( 0x80 );
                      
                      while( (temp=pgm_read_byte( &a[i]) ) != '\0' ) {
                                 lcd_put_8bit( temp );
                                 _delay_ms( 260 );
                                 if( i%(LCD_COL*2)==LCD_COL  -1 ) lcd_cmd_8bit( 0x80 + 0x40 );
                                 if( i%(LCD_COL*2)==LCD_COL*2-1 ) lcd_cmd_8bit( 0x01        );
                                 i++;
                      }
                                 _delay_ms( 1000 );
                      i=0;
                      lcd_cmd_8bit( 0x80 );
                      
                      while( (temp=pgm_read_byte( &b[i]) ) != '\0' ) {
                                 lcd_put_8bit( temp );
                                 _delay_ms( 30 );
                                 if( i%(LCD_COL*2)==LCD_COL  -1 ) lcd_cmd_8bit( 0x80 + 0x40 );
                                 if( i%(LCD_COL*2)==LCD_COL*2-1 ) lcd_cmd_8bit( 0x01        );
                                 i++;
                      }
                      i=0;
                      lcd_cmd_8bit( 0x80 );
                      while( (temp=pgm_read_byte( &c[i]) ) != '\0' ) {
                                 lcd_put_8bit( temp );
                                 _delay_ms( 260 );
                                 if( i%(LCD_COL*2)==LCD_COL  -1 ) lcd_cmd_8bit( 0x80 + 0x40 );
                                 if( i%(LCD_COL*2)==LCD_COL*2-1 ) lcd_cmd_8bit( 0x01        );
                                 i++;
                      }
                                 _delay_ms( 1000 );
                      i=0;
                      lcd_cmd_8bit( 0x80 );
                      
                      while( (temp=pgm_read_byte( &b[i]) ) != '\0' ) {
                                 lcd_put_8bit( temp );
                                 _delay_ms( 30 );
                                 if( i%(LCD_COL*2)==LCD_COL  -1 ) lcd_cmd_8bit( 0x80 + 0x40 );
                                 if( i%(LCD_COL*2)==LCD_COL*2-1 ) lcd_cmd_8bit( 0x01        );
                                 i++;
                      }
           }
           
           return 0;
}

