;; 
;; Tsuyoshi Uema  OKINAWA JAPAN
;; 
;; PCINT割り込み
;; 
;; LED OFF ->SW(PCINT2) -> LED ON  ( ONE SHOT )
;; RESET-SW-回路も組む
;; 
;; GND--SW----+
;;            |          *--------
;; VCC-[10K]--+---------|RESET VCC|------------------- VCC
;; {同じ}-- LED --------|PB3  INT0|----- SW ---------- GND
;; {同じ}-- LED --------|PB4   PB1|----- LED --------{同じ}
;; GND  ----------------|GND   PB0|----- LED --[330]-- GND
;;                       ---------
;;                        TINY85
;; 
;; PROGRAMER: USBASP
;; 8[MHz]
;; 
;; 
;; 電源投入後、LEDが点灯してしまいます。
;; パスコン　0.1uF, 電解コンデンサ10uFあたりを付け加えようと思います。
;; パスコンを追加したら、電源投入時にLEDOFFになるか、試してみます。
;; 電源投入時にLEDは消灯にならなければいけない、コードです。
;; 今は、電源投入後リセットを押して実験をしています。
;;
   
   
   
   

.include "../AVR_DEF/tn85def.inc"
.include "../MACRO/macro.inc"


.ORG 0
	rjmp RESET                          ;各種リセット
	reti	;;; rjmp INT0_ISR           ; 外部割り込み要求0
	rjmp PCINT0_ISR                     ; ピン変化割り込み要求
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
	outp SPH,    HIGH( RAMEND )
	outp SPL,    LOW( RAMEND )
	outp PCMSK,  (1<<PCINT2)            ; PB2 == PCINT2 変化割り込み
	outp GIMSK,  (1<<PCIE)              ; PIN変化割り込み
	outp DDRB,   0b11011                ; PB2=INPUT, OTHER=OUTPUT
	outp PORTB,  0b00100                ; PB2: PULLUP
	sei                                 ; 全割り込み許可
MAIN:
	rjmp MAIN                           ;;; MAIN へ戻る


PCINT0_ISR:                             ; ピン変化割り込み要求
	push R16
	in   R16,   SREG
	;;;;;;;;;;;;;;;;;;;;;;;;;
	outp PORTB, 0b11111                 ; LED ON
	;;;;;;;;;;;;;;;;;;;;;;;;;
	out  SREG,  R16
	pop  R16
	reti

;;; INT0_ISR: reti          ; 外部割り込み要求0
;;; TIM1_COMPA_ISR: reti    ; タイマ/カウンタ1比較A一致
;;; TIM1_OVF_ISR: reti      ; タイマ/カウンタ1溢れ
;;; TIM0_OVF_ISR: reti      ; タイマ/カウンタ0溢れ
;;; EE_RDY_ISR: reti        ; EEPROM操作可
;;; ANA_COMP_ISR: reti      ; アナログ比較器出力遷移
;;; ADC_ISR: reti           ; A/D変換完了
;;; TIM1_COMPB_ISR: reti    ; タイマ/カウンタ1比較B一致
;;; TIM0_COMPA_ISR: reti    ; タイマ/カウンタ0比較A一致
;;; TIM0_COMPB_ISR: reti    ; タイマ/カウンタ0比較B一致
;;; WDT_OVF_ISR: reti       ; ウォッチドッグ計時完了
;;; USI_START_ISR: reti     ; USI開始条件検出
;;; USI_OVF_ISR: reti       ; USI計数器溢れ
