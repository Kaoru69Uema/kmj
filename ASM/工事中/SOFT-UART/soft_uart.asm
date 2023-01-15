;; Soft_UART    上間つよし＠沖縄県西原町＠久米島出身
;;
;; CLOCK: 1MHz(内臓)
;;
;; +-------------------------------------+
;; | +---------------------------------+ |
;; | |                                 | |
;; | |    *---------                   | |    *--------- 
;; | | --|RESET  VCC|------------VCC   | | --|RESET  VCC|------------VCC
;; | +---|PB3    PB2|--                | *---|PB3    PB2|--
;; +-----|PB4    PB1|--SW--------GND   +-----|PB4    PB1|--SW--------GND
;;GND----|GND    PB0|--LED-[330]-GND  GND----|GND    PB0|--LED-[330]-GND
;;        ----------                         ----------
;;         tiny85-A                            tiny85-B
;;
;; ----------------------------------------------------------------
;;   PB3(RXD)-----------------+                tiny85-B)
;;                            |
;;   PB4(TXD)----------------------------------PB3(RXD)
;;                            |
;;   tiny85-A                 +----------------PB4(TXD)
;;
;;   SW                                        SW
;;
;;   LED                                       LED
;; ----------------------------------------------------------------
;; 説明：
;; A のSWを押している間、B のLEDが点灯
;; B のSWを押している間、A のLEDが点灯
;; ----------------------------------------------------------------
.nolist
.include "../AVR_DEF/tn85def.inc"
.list
.listmac

.macro outp           ;;; usage: outp TCNT0, 0
	ldi R16, @1
	out @0, R16
.endmacro

.equ LED = PB0	; OUTPUT
.equ SW  = PB1  ; INPUT, PULLUP
         ; PB2  ; NOT USED
.equ RXD = PB3  ; INPUT, PULLUP
.equ TXD = PB4  ; OUTPUT

.org 0
	rjmp RESET

.org OC0Aaddr
	rjmp COMP0A_ISR
;;-----------------------------------------------
.org INT_VECTORS_SIZE
RESET:
	sbi DDRB,  LED
	sbi DDRB,  TXD
	sbi PORTB, SW
	sbi PORTB, RXD
	;;; 1.0/9600=104.1666666[us]
	;;; 1.0/1000000=1.0[us]  (TCNT0は1[us]毎にインクリメントされる)
	;;; Timer0 CTC動作での比較値は、104-1=103になる。CLOCK分周なし
	;;; 少々の誤差は許容される
	outp TCCR0A, 1<<WGM01     ;;; Timer0 CTC動作
	outp TCCR0B, 1<<CS00      ;;; CLOCK/1 分周なし
	outp TIMSK,  1<<OCIE0A    ;;; 比較一致割り込み
	outp OCR0A, 103           ;;; 1.0/9600[s]毎に割り込み発生
	outp TCNT0, 0
	;SEI                       ;;; 全割り込み許可
MAIN:
	;sbis PINB, SW
	;rjmp ON
	;rjmp OFF
MAIN9:
	RJMP MAIN

ON:
	SBI  PORTB, LED
	rjmp MAIN9

OFF:
	CBI  PORTB, LED
	rjmp MAIN9


COMP0A_ISR:
	SBI  PORTB, LED
	reti
