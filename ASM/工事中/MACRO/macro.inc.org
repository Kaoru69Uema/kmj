; 作者　上間健
; バグも含んでいます。あまり参考になりません。


.macro outp
	ldi R16, @1
	out @0, R16
.endmacro
.macro ldix
	ldi R16, @1
	mov @0, R16
.endmacro
.macro andix
	ldi R16, @1
	and @0, R16
.endmacro
.macro orix
	ldi R16, @1
	or @0, R16
.endmacro
.macro eorix
	ldi R16, @1
	eor @0, R16
.endmacro
.macro cpix
	ldi R16, @1
	cp @0, R16
.endmacro
.macro cpi16x        ; usage: cpi16x registerH, registerL, 1000
	ldi R16, LOW( @2 )
	cp @1, R16
	ldi R16, HIGH( @2 )
	cpc @0, R16
.endmacro
.macro inccounter1000
	adiw R26, 1    ; inc R27:R26
.endmacro
.macro delay
	ldi R16, @0
delay1:
	push R16
	nop
	nop
	nop
	nop
	nop
	nop
	dec R16
	brne delay1
	pop  R16
.endmacro
