


; AVR {in/lds}  {out/sts} automatic selection macros
; 	The device include file must be used to define SRAM_START


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.macro outp
  PUSH R16
	ldi R16, @1
	out @0, R16
  POP R16
.endm


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; usage: InReg reg, addr

.macro InReg
	.if @1 < 0x40
		in @0, @1
	.elif ( (@1 >= 0x60) && (@1 < SRAM_START) )
		lds @0, @1
	.else
		.error "InReg: Invalid I/O register address"
	.endif
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; usage: OutReg addr, reg

.macro OutReg
  PUSH R16
	ldi R16, @1
	.if @0 < 0x40
		out @0, R16
	.elif ( (@0 >= 0x60) && (@0 < SRAM_START) )
		sts @0, R16
	.else
		.error "OutReg: Invalid I/O register address"
	.endif
  POP R16
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
