;; Timer0:CTC:LED NIGHT RAIDER     Tsuyoshi Uema  OKINAWA JAPAN
;; 
;;               *--------
;;    RESET-----|RESET VCC|----- VCC
;;    LED  -----|PB3   PB2|----- LED
;;    LED  -----|PB4   PB1|----- LED
;;    GND  -----|GND   PB0|----- LED
;;               ---------
;;                TINY85
;; 
;; PROGRAMER: USBASP
;; 8[MHz]
;; 
;; 
;; 
;; 
;; 
;; 

.INCLUDE "../AVR_DEF/tn85def.inc"
.include "../MACRO/macro.inc"

.def temp = R16
.def flag = R17
.def LED = R18
.equ f1ms=0
.equ direction=1

.ORG 0
	rjmp RESET              ;各種リセット
	reti	;;; rjmp INT0_ISR           ; 外部割り込み要求0
	reti	;;; rjmp PCINT0_ISR         ; ピン変化割り込み要求
	reti	;;; rjmp TIM1_COMPA_ISR     ; タイマ/カウンタ1比較A一致
	reti	;;; rjmp TIM1_OVF_ISR       ; タイマ/カウンタ1溢れ
	reti	;;; rjmp TIM0_OVF_ISR       ; タイマ/カウンタ0溢れ
	reti	;;; rjmp EE_RDY_ISR         ; EEPROM操作可
	reti	;;; rjmp ANA_COMP_ISR       ; アナログ比較器出力遷移
	reti	;;; rjmp ADC_ISR            ; A/D変換完了
	reti	;;; rjmp TIM1_COMPB_ISR     ; タイマ/カウンタ1比較B一致
	rjmp TIM0_COMPA_ISR                 ; タイマ/カウンタ0比較A一致
	reti	;;; rjmp TIM0_COMPB_ISR     ; タイマ/カウンタ0比較B一致
	reti	;;; rjmp WDT_OVF_ISR        ; ウォッチドッグ計時完了
	reti	;;; rjmp USI_START_ISR      ; USI開始条件検出
	reti	;;; rjmp USI_OVF_ISR        ; USI計数器溢れ
;;;

RESET:             ;各種リセット
	outp SPH,    HIGH( RAMEND )
	outp SPL,    LOW( RAMEND )
	outp DDRB,   0b11111
	outp TCCR0A, 1<<WGM01      ; ck/64 ctc-timer
	outp TCCR0B, 0b011<<CS00   ; ck/64 ctc-timer
	outp TIMSK,  1<<OCIE0A      ; TIM0_COMPA_ISR
	outp OCR0A,  8000000/64/1000-1
	outp TCNT0,  0
	sei
	ldi LED,     1
MAIN:                      ;;; 1[ms] wait
	sbrs flag,   f1ms      ;;; 1[ms] wait
	rjmp MAIN
	cbr flag,    1<<f1ms   ;;; flag reset
	inccounter1000         ;;; R27:R26 incriment
	cpi16x R27, R26, 200   ;;; if 200[ms]  ?
	brne MAIN9             ;;;    else goto MAIN9
	clr R26                ;;;    then R26 <= 0
	clr R27                ;;;    then R27 <= 0
	sbrs flag,   direction ;;; if 方向flag==1
	rjmp left              ;;;   else  goto left
	rjmp right             ;;;   then  goto right
MAIN9:
	out PORTB,   LED       ;;; output LED -> PORTB
	rjmp MAIN              ;;; MAIN へ戻る

left:
	lsl LED                ;;; 左シフト R7 <- R6 ... <- R0 <- 0
	cpi LED,     0b10000   ;;; if LED == 0b10000 ?
	brne left9             ;;;    else goto left9
	sbr flag,    1<<direction ;; then 方向フラグ <= 1
	ldi LED,     0b10000   ;; then LED <= 0b100000
left9:
	rjmp MAIN9             ;;; goto MAIN9

right:
	lsr LED                ;;; 右シフト 0 -> R7 -> R6 ...R1 -> R0
	cpi LED, 0b00001       ;;; if LED == 0b00001 ?
	brne right9            ;;;    else goto right9
	cbr flag, 1<<direction ;;;    then 方向flag <= 0
	ldi LED, 0b00001       ;;;    then LED <= 0b00001
right9:
	rjmp MAIN9             ;;; goto MAIN9


;;; INT0_ISR: reti          ; 外部割り込み要求0
;;; PCINT0_ISR: reti        ; ピン変化割り込み要求
;;; TIM1_COMPA_ISR: reti    ; タイマ/カウンタ1比較A一致
;;; TIM1_OVF_ISR: reti      ; タイマ/カウンタ1溢れ
;;; TIM0_OVF_ISR: reti      ; タイマ/カウンタ0溢れ
;;; EE_RDY_ISR: reti        ; EEPROM操作可
;;; ANA_COMP_ISR: reti      ; アナログ比較器出力遷移
;;; ADC_ISR: reti           ; A/D変換完了
;;; TIM1_COMPB_ISR: reti    ; タイマ/カウンタ1比較B一致

TIM0_COMPA_ISR:             ; タイマ/カウンタ0比較A一致
	sbr flag, 1<<f1ms
	reti

;;; TIM0_COMPB_ISR: reti    ; タイマ/カウンタ0比較B一致
;;; WDT_OVF_ISR: reti       ; ウォッチドッグ計時完了
;;; USI_START_ISR: reti     ; USI開始条件検出
;;; USI_OVF_ISR: reti       ; USI計数器溢れ
