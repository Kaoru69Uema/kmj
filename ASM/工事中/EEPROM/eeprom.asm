;; 
;; Tsuyoshi Uema  OKINAWA JAPAN
;; 
;; EEPROM
;; 
;; RESET-SW-回路も組む
;; 
;; GND--SW----+
;;            |          *--------
;; VCC-[10K]--+---------|RESET VCC|------------------- VCC
;; {同じ}-- LED --------|PB3   PB2|----- LED --------{同じ}
;;              --------|PB4   PB1|----- LED --------{同じ}
;; GND  ----------------|GND   PB0|----- LED --[330]-- GND
;;                       ---------
;;                        TINY85
;; 
;; PROGRAMER: USBASP
;; 8[MHz]
;; 
;; 
;; ATtiny85 は、512バイトのデータ用EEPROMが内蔵されています。
;; それは、単一バイトで読み書きできます。
;; 
;; 
   
   
   

.include "../AVR_DEF/tn85def.inc"

.def REG_EEAR_H = R18
.def REG_EEAR_L = R17
.def REG_EEDR_R = R20
.def REG_EEDR_W = R21
.equ EEPROMAR   = 1000

.macro outp
	ldi R16, @1
	out @0, R16
.endmacro


.ORG 0
	rjmp RESET                          ;各種リセット
	reti	;;; rjmp INT0_ISR           ; 外部割り込み要求0
	reti	;;; rjmp PCINT0_ISR         ; ピン変化割り込み要求
	reti	;;; rjmp TIM1_COMPA_ISR     ; タイマ/カウンタ1比較A一致
	reti	;;; rjmp TIM1_OVF_ISR       ; タイマ/カウンタ1溢れ
	reti	;;; rjmp TIM0_OVF_ISR       ; タイマ/カウンタ0溢れ
	reti	;;; rjmp EE_RDY_ISR         ; EEPROM操作可
	reti	;;; rjmp ANA_COMP_ISR       ; アナログ比較器出力遷移
	reti	;;; rjmp ADC_ISR            ; A/D変換完了
	reti	;;; rjmp TIM1_COMPB_ISR     ; タイマ/カウンタ1比較B一致
	reti	;;; rjmp TIM0_COMPA_ISR     ; タイマ/カウンタ0比較A一致
	reti	;;; rjmp TIM0_COMPB_ISR     ; タイマ/カウンタ0比較B一致
	reti	;;; rjmp WDT_OVF_ISR        ; ウォッチドッグ計時完了
	reti	;;; rjmp USI_START_ISR      ; USI開始条件検出
	reti	;;; rjmp USI_OVF_ISR        ; USI計数器溢れ
;;;

RESET:
	outp  SPH,      HIGH( RAMEND )
	outp  SPL,      LOW( RAMEND )
	outp  DDRB,     0b01111           ; PB4:INPUT, OTHER:OUTPUT
	outp  PORTB,    0b10000           ; PB4:PULLUP 
	ldi   REG_EEAR_H, high(0xff)  ; アドレス設定
	ldi   REG_EEAR_L, low( 0xff)  ; アドレス設定
	ldi   REG_EEDR_W, 0b1111            ; データ値設定
	rcall EEPROM_WR        ; EEPROMへデータ値書き込む
	rcall EEPROM_RD        ; EEPROMからデータを読み込む
	out   PORTB, REG_EEDR_R; PORTBへ読み込んだデータ値を出力する
MAIN:
	rjmp MAIN              ;;; MAIN へ戻る

EEPROM_WR:
	SBIC EECR,EEPE    ; EEPROMプログラミング完了ならばスキップ
	RJMP EEPROM_WR    ; 以前のEEPROMプログラミング完了まで待機
	;
	LDI R19,(0<<EEPM1)|(0<<EEPM0) ;プログラミング種別値取得(本例は非分離)
	OUT EECR,   R19   ; 対応プログラミング種別設定
	OUT EEARH,  REG_EEAR_H ; EEPROMアドレス上位バイト設定
	OUT EEARL,  REG_EEAR_L ; EEPROMアドレス下位バイト設定
	OUT EEDR,   REG_EEDR_W ; EEPROM書き込み値を設定
	SBI EECR,   EEMPE ; EEPROM主プログラム許可ビット設定
	SBI EECR,   EEPE  ; EEPROMプログラミング開始(プログラム許可ビット設定)
	RET               ; 呼び出し元へ復帰

EEPROM_RD: 
	SBIC EECR,      EEPE ;EEPROMプログラミング完了ならばスキップ
	RJMP EEPROM_RD       ;以前のEEPROMプログラミング完了まで待機
	;
	OUT EEARH,      REG_EEAR_H   ;EEPROMアドレス上位バイト設定
	OUT EEARL,      REG_EEAR_L   ;EEPROMアドレス下位バイト設定
	SBI EECR,       EERE ;EEPROM読み出し開始(読み込み許可ビット設定)
	IN  REG_EEDR_R, EEDR ;EEPROM読み出し値を取得
	RET                  ;呼び出し元へ復帰


;;; INT0_ISR:        reti    ; 外部割り込み要求0
;;; PCINT0_ISR:      reti    ; ピン変化割り込み要求
;;; TIM1_COMPA_ISR:  reti    ; タイマ/カウンタ1比較A一致
;;; TIM1_OVF_ISR:    reti    ; タイマ/カウンタ1溢れ
;;; TIM0_OVF_ISR:    reti    ; タイマ/カウンタ0溢れ
;;; EE_RDY_ISR:      reti    ; EEPROM操作可
;;; ANA_COMP_ISR:    reti    ; アナログ比較器出力遷移
;;; ADC_ISR:         reti    ; A/D変換完了
;;; TIM1_COMPB_ISR:  reti    ; タイマ/カウンタ1比較B一致
;;; TIM0_COMPA_ISR:  reti    ; タイマ/カウンタ0比較A一致
;;; TIM0_COMPB_ISR:  reti    ; タイマ/カウンタ0比較B一致
;;; WDT_OVF_ISR:     reti    ; ウォッチドッグ計時完了
;;; USI_START_ISR:   reti    ; USI開始条件検出
;;; USI_OVF_ISR:     reti    ; USI計数器溢れ
