; ２つのincファイルを現在のディレクトリにコピーしました。
; ３回照合ではチャタリングを外すことができない場合がありました。
; ４回照合に変更します。今から取り掛かります。
; 2022年11月11日午後7時36分〜

.NOLIST
.INCLUDE "tn85def.inc"
.LIST
.LISTMAC
.INCLUDE "macro.inc"

.ORG 0
RJMP RESET
; ***** INTERRUPT VECTORS ************************************************
.ORG    INT0ADDR        ;EXTERNAL INTERRUPT 0
  reti
.ORG    PCI0ADDR        ;PIN CHANGE INTERRUPT REQUEST 0
  reti
.ORG    OC1AADDR        ;TIMER/COUNTER1 COMPARE MATCH 1A
  reti
.ORG    OVF1ADDR        ;TIMER/COUNTER1 OVERFLOW
  reti
.ORG    OVF0ADDR        ;TIMER/COUNTER0 OVERFLOW
  reti
.ORG    ERDYADDR        ;EEPROM READY
  reti
.ORG    ACIADDR         ;ANALOG COMPARATOR
  reti
.ORG    ADCCADDR        ;ADC CONVERSION READY
  reti
.ORG    OC1BADDR        ;TIMER/COUNTER1 COMPARE MATCH B
  reti
.ORG    OC0AADDR        ;TIMER/COUNTER0 COMPARE MATCH A
  rjmp  COMPA0
.ORG    OC0BADDR        ;TIMER/COUNTER0 COMPARE MATCH B
  reti
.ORG    WDTADDR         ;WATCHDOG TIME-OUT
  reti
.ORG    USI_STARTADDR   ;USI START
  reti
.ORG    USI_OVFADDR     ;USI OVERFLOW
  reti

.ORG    INT_VECTORS_SIZE    ;SIZE IN WORDS

; 名称設定
.EQU    SW      =   PB4
.EQU    LED     =   PB3

; レジスタ別名設定
.DEF    SV_SREG =   R1   ; SREG 退避用
.DEF    SW_NOW  =   R2   ; SW 入力値
.DEF    SW_CNT  =   R3   ; SW カウンタ
.DEF    SW_KAK  =   R4   ; SW_確定値
.DEF    SW_Z__  =   R5   ; SW_前回値
.DEF    SW_ZZ_  =   R6   ; SW_前々回値
.DEF    SW_ZZZ  =   R7   ; SW_前々々回値         ; 追加(４回照合)
.DEF    SW_ZZZZ  =   R8  ; SW_前々々回値         ; 追加(４回照合)
.DEF    SW_UP_  =   R9   ; SW_立ち上がり・フラグ
.DEF    SW_DN_  =   R10  ; SW_立ち下がり・フラグ
.DEF    SW_TMP  =   R11  ; SW_一時レジスタ
.DEF    F_1MS   =   R12  ; 1[ms] フラグ
.DEF    F_200US =   R13  ; 200[us] フラグ
.DEF    TMP     =   R16  ; 一時：レジスタ

RESET:
  CLI
  ; IO SETTING
  SBI  DDRB,   LED                ; ポート　出力設定
  SBI  PORTB,  SW                 ; スイッチプルアップ

  ; TIMER0 SETTING
  OUTP  TCCR0A, (1<<WGM01)              ; タイマー: CTC動作
  OUTP  TCCR0B, (1<<CS01)               ; クロック分周 ck/8
  OUTP  OCR0A,  (8000000/8/5000-1)      ; 1/5000[s] = 200[us]
  OUTP  TIMSK,  (1<<OCIE0A)             ; 比較一致A割込み許可
  SEI                                   ; 全割り込み許可
MAIN:
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; TIMMING WAIT 1ms
  TST   F_1MS           ; ゼロのテスト
  BREQ  END9            ; ゼロなので何もせずに抜ける。
  CLR   F_1MS           ; フラグ・リセット
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; TIMMING WAIT 1ms
  RCALL SW_MAIN         ; スイッチ入力（３回照合）
  SBRC  SW_DN_,  SW     ; 押された？
  RJMP  LED_ON          ; LEDオン
  SBRC  SW_UP_,  SW     ; 離された？
  RJMP  LED_OFF         ; LEDオフ
END9:
  RJMP MAIN

SW_MAIN:
  IN    SV_SREG,  SREG    ; ステータスレジスタ保存
  PUSH  TMP               ; 一時レジスタ退避
  CLR   SW_DN_            ; 立ち下がりフラグ・リセット
  CLR   SW_UP_            ; 立ち上がりフラグ・リセット
  INC   SW_CNT            ; １０ミリ秒カウンタ・インクリメント
  LDI   TMP, 12           ; 比較用レジスタセット
  CP    SW_CNT, TMP       ; １０カウント？
  BRNE  SW_END            ; １０回に達していなければ抜ける
  CLR   SW_CNT            ; カウンター・リセット

  INREG TMP,  PINB        ; スイッチの状態を入力
  ANDI  TMP,  (1<<SW)     ; 不要ビット削除
  MOV   SW_NOW, TMP       ; 今回値を保存
  OR    TMP,  SW_ZZZZ      ; (今回値 or 前々々回値 or 前々回値 & 前回値) &  確定値 ; 追加
  OR    TMP,  SW_ZZZ      ; (今回値 or 前々々回値 or 前々回値 & 前回値) &  確定値 ; 追加
  OR    TMP,  SW_ZZ_      ; 上の続き
  OR    TMP,  SW_Z__      ; 上の続き
  AND   TMP,  SW_KAK      ; 上の続き
  MOV   SW_TMP, TMP       ; 計算した途中計算結果を保存
  MOV   TMP,  SW_NOW      ; 今回値をコピー
  AND   TMP,  SW_ZZZZ      ; (今回値 and 前々々回値 and 前々回値 and 前回値) or (上の途中結果) ; 追加
  AND   TMP,  SW_ZZZ      ; (今回値 and 前々々回値 and 前々回値 and 前回値) or (上の途中結果) ; 追加
  AND   TMP,  SW_ZZ_      ; 上の続き
  AND   TMP,  SW_Z__      ; 上の続き
  OR    SW_TMP, TMP       ; 計算完了：新確定値
  MOV   SW_TMP, SW_KAK    ; 前の確定値保存
  MOV   SW_KAK, TMP       ; 計算した新確定値を保存
  MOV   TMP,  SW_TMP      ; 立ち上がり＝〜前確定値 & 新確定値
  COM   TMP               ; 上の続き
  AND   TMP,  SW_KAK      ; 上の続き
  MOV   SW_UP_, TMP       ; 立ち上がりフラグ保存
  MOV   TMP,  SW_KAK      ; 立ち下がり＝前確定値 & 〜新確定値
  COM   TMP               ; 上の続き
  AND   SW_TMP, TMP       ; 上の続き
  MOV   SW_DN_, SW_TMP    ; 立ち下がりフラグ保存
  MOV   SW_ZZZZ, SW_ZZZ    ; 前々々回 <= 前々回
  MOV   SW_ZZZ, SW_ZZ_    ; 前々々回 <= 前々回
  MOV   SW_ZZ_, SW_Z__    ; 前々回 <= 前回
  MOV   SW_Z__, SW_NOW    ; 前回   <= 今回
SW_END:
  POP   TMP               ; 一時レジスタ復帰
  OUT   SREG,   SV_SREG   ; シフトレジスタ戻す
  RET

LED_ON:                   ; LEDオン
  SBI PORTB, LED
  rjmp END9

LED_OFF:                  ; LEDオフ
  CBI PORTB, LED
  rjmp END9

COMPA0:                   ; ２００マイクロ秒毎に呼び出される
  IN      SV_SREG,  SREG  ; １ミリ秒フラグ立てる
  PUSH    TMP
  INC     F_200US
  LDI     TMP, 5
  CP      TMP,  F_200US
  BRNE    COMPA0_END
  CLR     F_200US
  INC     F_1MS
COMPA0_END:
  POP     TMP
  OUT     SREG, SV_SREG
  reti

