;; AC           上間つよし＠沖縄県西原町＠久米島出身
;; 
;; アナログコンパレーター 内部基準電圧1.1[v]とPB1の比較
;; 結果をPB4 LEDへ出力                                      VCC
;;                    *------------------------              |
;; VCC-[10k] --------|RESET                 VCC|---- VCC     |
;;               ----|PB3/~OC1B        OC1A/PB2|----         |
;; GND --[330]-LED---|PB4/OC1B         OC0B/PB1|----------[Volume]
;; GND --------------|GND        OC0A/~OC1A/PB0|----+        |
;;                    -------------------------     |        |
;;                             TINY85               |  VCC  GND
;;                                                  |   |
;;                                                  |  [10k]
;; PROGRAMER: USBASP                                |   |
;; 8[MHz]                                           +---+
;;                                                      |
;;                                                     [10k]
;;                                                      |
;;                                                     GND
    ;;;>>> アナログコンパレータ
    ;;;>>>（内部1.1[v]） 比較 （PB1の電圧）  ：　プログラミング
    ;;;>>> (PB0)   比較 （PB1の電圧）　　　　：　コメントアウト
    ;;;>>> 比較結果を(PB4 LED)へ出力

   ;;;--- アナログコンパレータは常時動いているようです ---

;===========================================================ここから 比較値を　PB0の電圧とした場合
.include "../AVR_DEF/tn85def.inc"

.MACRO OUTP
	LDI R16, @1
	OUT @0, R16
.ENDMACRO

.org 0
RESET:
	outp DDRB, 0b00010000
MAIN:
	in R16, ACSR       ; 結果ACOを見てPORTB出力を変える
	sbrs R16, ACO      ; 結果ACOを見てPORTB出力を変える
	rjmp A
	rjmp B
MAIN9:
	RJMP MAIN

A:	sbi  PORTB, PB4
	rjmp MAIN9

B:	cbi  PORTB, PB4
	rjmp MAIN9


;===========================================================ここまで





;=========================================================== コメントアウト部分は、比較値を内部　1.1[v]とした場合のコード

;;; .include "../AVR_DEF/tn85def.inc"
;;; 
;;; .MACRO OUTP
;;; 	LDI R16, @1
;;; 	OUT @0, R16
;;; .ENDMACRO
;;; 
;;; .org 0
;;; RESET:
;;; 	outp ACSR, 1<<ACBG  ; 内部基準電圧1.1[v]選択
;;; 	outp DDRB, 0b00010000
;;; MAIN:
;;; 	in R16, ACSR       ; 結果ACOを見てPORTB出力を変える
;;; 	sbrs R16, ACO      ; 結果ACOを見てPORTB出力を変える
;;; 	rjmp A
;;; 	rjmp B
;;; MAIN9:
;;; 	RJMP MAIN
;;; 
;;; A:	sbi  PORTB, PB4
;;; 	rjmp MAIN9
;;; 
;;; B:	cbi  PORTB, PB4
;;; 	rjmp MAIN9
;;; 
;;; 
