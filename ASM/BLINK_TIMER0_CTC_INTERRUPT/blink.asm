.include "../AVR_DEF/tn85def.inc"
.include "../MACRO/macro.inc"

.def temp = R16
.def flag = R17
.def LED  = R18
.equ f1ms = 0

.ORG 0
rjmp RESET              ;各種リセット
rjmp INT0_ISR           ;外部割り込み要求0
rjmp PCINT0_ISR         ;ピン変化割り込み要求
rjmp TIM1_COMPA_ISR     ;タイマ/カウンタ1比較A一致
rjmp TIM1_OVF_ISR       ;タイマ/カウンタ1溢れ
rjmp TIM0_OVF_ISR       ;タイマ/カウンタ0溢れ
rjmp EE_RDY_ISR         ;EEPROM操作可
rjmp ANA_COMP_ISR       ;アナログ比較器出力遷移
rjmp ADC_ISR            ;A/D変換完了
rjmp TIM1_COMPB_ISR     ;タイマ/カウンタ1比較B一致
rjmp TIM0_COMPA_ISR     ;タマi/カウンタ0比較A一致
rjmp TIM0_COMPB_ISR     ;タイiマ/カウンタ0比較B一致
rjmp WDT_OVF_ISR        ;ウォッチドッグ計時完了
rjmp USI_START_ISR      ;USI開始条件検出
rjmp USI_OVF_ISR        ;USI計数器溢れ
;;;

RESET:             ;各種リセット
	outp SPH, HIGH( RAMEND )
	outp SPL, LOW( RAMEND )
	ldi temp, 0xff
	out DDRB, temp
	outp TCCR0A, 1<<WGM01      ; ck/64 ctc-timer
	outp TCCR0B, 0b011<<CS00   ; ck/64 ctc-timer
	outp TIMSK, 1<<OCIE0A      ; TIM0_COMPA_ISR
	outp OCR0A, 8000000/64/1000-1
	outp TCNT0, 0
	sei
MAIN:
	sbrs flag, f1ms  ;;; 1[ms] wait
	brne MAIN        ;;; 1[ms] wait
	cbr flag, 1<<f1ms  ;;; flag reset
	inccounter1000         ;;; R27:R26 increment
	cpi16x R27, R26, 1000  ;;; if 1000[ms] ?
	brne MAIN9             ;;;      else  goto MAIN9
	clr R27                ;;;      then  clear R27
	clr R26                ;;;      then  clear R26
	com LED                ;;;      then  LED all but  toggle
	out PORTB, LED         ;;;      then  output LED
MAIN9:
	rjmp MAIN



INT0_ISR: reti          ;外部割り込み要求0
PCINT0_ISR: reti        ;ピン変化割り込み要求
TIM1_COMPA_ISR: reti    ;タイマ/カウンタ1比較A一致
TIM1_OVF_ISR: reti      ;タイマ/カウンタ1溢れ
TIM0_OVF_ISR: reti          ;タイマ/カウンタ0溢れ
EE_RDY_ISR: reti        ;EEPROM操作可
ANA_COMP_ISR: reti      ;アナログ比較器出力遷移
ADC_ISR: reti           ;A/D変換完了
TIM1_COMPB_ISR: reti    ;タイマ/カウンタ1比較B一致
TIM0_COMPA_ISR:         ;タマi/カウンタ0比較A一致
	sbr flag, 1<<f1ms
	reti

TIM0_COMPB_ISR: reti    ;タイiマ/カウンタ0比較B一致
WDT_OVF_ISR: reti       ;ウォッチドッグ計時完了
USI_START_ISR: reti     ;USI開始条件検出
USI_OVF_ISR: reti       ;USI計数器溢れ
