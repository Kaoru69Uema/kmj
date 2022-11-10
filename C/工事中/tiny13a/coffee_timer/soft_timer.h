#if !(defined(__SOFT_TIMER__))
#define __SOFT_TIMER__

/* 60分程度のソフトタイマ４本 */

extern unsigned long t_counter[ 4 ];         /* タイマカウンタ    */

void t_ini( void );                            /* 初期化       */
void t_main( void );                           /* メイン       */
void t_req( unsigned char i );                 /* リクエスト   */
void t_can( unsigned char i );                 /* キャンセル   */
unsigned char t_tup( unsigned char i );        /* タイムアップ */
unsigned char t_off( unsigned char i );        /* 休止中 */
unsigned char t_run( unsigned char i );        /* カウントダウン中 */
void t_set( unsigned char i, unsigned long n ); /* 強制設定     */
#endif


