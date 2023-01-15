.nolist
.device AT90S2313
.include "2313def.inc"
.list
.listmac
.include "macro.inc"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.equ    LED       =   PB7
.equ    SW        =   PD6
.equ    Sounder   =   PB3
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.def    sreg_save =   R0  ; sreg退避用
.def    sw_z      =   R1
.def    sw_zz     =   R2
.def    sw_kak    =   R3
.def    sw_cntr   =   R4
.def    sw_kon    =   R5
.def    sw_up     =   R6
.def    sw_dn     =   R7
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.def    temp      =   R16
.def    temp1     =   R17
.def    temp2     =   R18
.def    temp3     =   R19
.def    temp4     =   R20
.def    f_200us   =   R24
.def    f_1ms     =   R25
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.cseg
.org 0
	rjmp initialize
; ***** INTERRUPT VECTORS ************************************************
.org    INT0addr     ; External Interrupt Request 0
	reti
.org    INT1addr     ; External Interrupt Request 1
	reti
.org    ICP1addr     ; Timer/Counter1 Capture Event
	reti
.org    OC1addr      ; Timer/Counter1 Compare Match
	reti
.org    OVF1addr     ; Timer/Counter1 Overflow
	reti
.org    OVF0addr     ; Timer/Counter0 Overflow
	rjmp OVFvect
.org    URXCaddr     ; UART, Rx Complete
	reti
.org    UDREaddr     ; UART Data Register Empty
	reti
.org    UTXCaddr     ; UART, Tx Complete
	reti
.org    ACIaddr      ; Analog Comparator
	reti

.org    INT_VECTORS_SIZE
initialize:
	cli
	outp	SPL,low(RAMEND)	; 設定：スタックポインタをRAMの上アドレスに設定

	;; IO-DIRECTION
	OutReg DDRB,  ( 1 << LED ) | ( 1 << Sounder )
	OutReg PORTD, ( 1 << SW )   ; Pull_UP

	;; Timer1 Setting
	OutReg TCCR1A, ( 1 << COM1A0 );;; |  ( 1 << PWM10 )
	OutReg TCCR1B, ( 1 << CTC1 ) | ( 1 << CS10 )
	OutReg OCR1AH,  High( 10000000/1/(888*2) )
	OutReg OCR1AL,  Low( 10000000/1/(888*2) )

	;; Timer0 Setting
	OutReg TCCR0, ( 1 << CS01 )    ;;;  ck/8
	OutReg TIMSK, ( 1 << TOIE0 )   ;;; Timer0 OVF inturrupt
	OutReg TCNT0, ( 256 - ( 10000000/8/5000 ) )

	;; All Inturrupt Enable
	sei

;---------------------------------------------------
main:
        cpi f_1ms, 0
        breq main
	clr  f_1ms

	rcall sw_main

	sbrc sw_kon, SW
	rjmp on
	rjmp off

main9:
	rjmp main
;---------------------------------------------------

on:
	sbi  PORTB, LED
	rjmp main9

;---------------------------------------------------
off:
	cbi  PORTB, LED
	rjmp main9

;---------------------------------------------------

sw_main:
	clr sw_dn
	clr sw_up
	dec sw_cntr
	brne sw_end
	ldi temp, 10
	mov sw_cntr, temp
	in sw_kon, PIND
	ldi temp, ( 1 << SW )
	and sw_kon, temp
	;;;３回照合;;;
	;;;;;;;;;;;;;;;;;;;;;;;;
sw_end:
	ret

OVFvect:
	in   sreg_save, sreg
	OutReg TCNT0, ( 256 - ( 10000000/8/5000 ) )
	inc  f_200us
	cpi  f_200us, 5
	brne OVF_9
	ldi  f_1ms, 1
OVF_9:
	out sreg, sreg_save
	reti



.if 0
	ldi zl, low( var1 )
	ldi zh, high( var1 )
	ld  R1, Z
.endif
.if 0
	lds R2, var1
	sts var1, R2
.endif
