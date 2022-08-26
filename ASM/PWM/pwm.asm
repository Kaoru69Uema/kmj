;; PWM                                Tsuyoshi Uema   OKINAWA JAPAN
;; 8ビット高速PWM動作
;;
;;                    *------------------------
;; VCC-[10k] --------|RESET                 VCC|---- VCC
;;               ----|PB3/~OC1B        OC1A/PB2|----
;; GND ----- SW -----|PB4/OC1B         OC0B/PB1|---- LED -[330]- GND
;; GND --------------|GND        OC0A/~OC1A/PB0|---- LED -[330]- GND
;;                    -------------------------
;;                             TINY85
;; 
;; TCNT0 ....255,0,1,2,......253,254,255,0,1,..... でカウンタを回す
;; 
;; SW:Push ->   OCR0A =  30, OCR0B = 255 : PB1 のLEDの方が明るい
;; SW:OFF  ->   OCR0A = 255, OCR0B =  30 : PB0 のLEDの方が明るい
;; 
;; 
;; 
;; 
;; PROGRAMER: USBASP
;; 8[MHz]
;; 
;; 

.MACRO OUTP
	LDI R16, @1
	OUT @0, R16
.ENDMACRO

.include "../AVR_DEF/tn85def.inc"

	SBI DDRB,  PB0  ; 出力
	SBI DDRB,  PB1  ; 出力
	SBI PORTB, PB4  ; プルアップ( 入力 )

	OUTP TCCR0A, 0B10100011
;;;	               ||||||++--(WGM02),WGM01,WGM00:011
;;;	               ||||++----NOT_USE
;;;	               ||++------COM0B1,COM0B0:10  比較一致でLow、BOTTOMでHigh
;;;	               ++--------COM0A1,COM0A0:10  比較一致でLow、BOTTOMでHigh
	OUTP TCCR0B, (1 << CS00)  ; clk/1 分周なし
LOOP:
	SBIC PINB, PB4  ; SWが押されていたら次をスキップ
	RJMP BRITE1
	RJMP BRITE2
LOOP_9:
	RJMP LOOP

BRITE1:
	OUTP OCR0A, 255
	OUTP OCR0B,  30
	RJMP LOOP_9

BRITE2:
	OUTP OCR0A,  30
	OUTP OCR0B, 255
	RJMP LOOP_9

