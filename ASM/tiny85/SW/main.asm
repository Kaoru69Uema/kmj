;; スイッチ入力
;; ここでは５回照合を行いました
;; 
;;                 *--------
;; RESET----------|RESET VCC|------------VCC
;; GND-[330]-LED--|PB3   PB2|--
;; GND-------SW---|PB3   PB1|--
;; GND------------|GND   PB0|--
;;                 ---------
;;                  TINY85
;; 
;; PROGRAMER: HIDASPX  自作しました。
;; （ネットGITHUB情報IRUKA様：ありがとうございます。）
;;
;; 8[MHZ]
;; 
;; 

.INCLUDE "tn85def.inc"

.EQU F1MS    = 0
.DEF SV_SREG = R1
.DEF SW_DN   = R2
.DEF SW_UP   = R3
.DEF SW_NOW  = R4
.DEF SW_KAK  = R5
.DEF SW_ZZZZ = R6
.DEF SW_ZZZ  = R7
.DEF SW_ZZ   = R8
.DEF SW_Z    = R9
.DEF SW_TMP  = R10
.DEF SW_CNT  = R11

.DEF TMP     = R16
.EQU LED     = PB3
.EQU SW      = PB4
.DEF FLAG    = R18



.MACRO OUTP
	LDI R16, @1
	OUT @0, R16
.ENDMACRO

.ORG 0
	RJMP RESET                        ;各種リセット
	RETI	; RJMP INT0_ISR           ;外部割り込み要求0
	RETI	; RJMP PCINT0_ISR         ;ピン変化割り込み要求
	RETI	; RJMP TIM1_COMPA_ISR     ;タイマ/カウンタ1比較A一致
	RETI	; RJMP TIM1_OVF_ISR       ;タイマ/カウンタ1溢れ
	RETI	; RJMP TIM0_OVF_ISR       ;タイマ/カウンタ0溢れ
	RETI	; RJMP EE_RDY_ISR         ;EEPROM操作可
	RETI	; RJMP ANA_COMP_ISR       ;アナログ比較器出力遷移
	RETI	; RJMP ADC_ISR            ;A/D変換完了
	RETI	; RJMP TIM1_COMPB_ISR     ;タイマ/カウンタ1比較B一致
	RJMP TIM0_COMPA_ISR               ;タイマ/カウンタ0比較A一致
	RETI	; RJMP TIM0_COMPB_ISR     ;タイマ/カウンタ0比較B一致
	RETI	; RJMP WDT_OVF_ISR        ;ウォッチドッグ計時完了
	RETI	; RJMP USI_START_ISR      ;USI開始条件検出
	RETI	; RJMP USI_OVF_ISR        ;USI計数器溢れ
;;;

RESET:
	OUTP SPH, HIGH( RAMEND )
	OUTP SPL, LOW( RAMEND )
	OUTP  DDRB, (1<<LED)
  OUTP  PORTB, (1<<SW)
	OUTP TCCR0A, 1<<WGM01      ; CK/64 CTC-TIMER
	OUTP TCCR0B, 0B011<<CS00   ; CK/64 CTC-TIMER
	OUTP TIMSK,  1<<OCIE0A      ; TIM0_COMPA_ISR
	OUTP OCR0A,  8000000/64/1000-1
	OUTP TCNT0,  0
	SEI
MAIN:
	SBRS FLAG,   F1MS      ;;; 1[MS] WAIT
	BRNE MAIN              ;;; 1[MS] WAIT
	CBR FLAG,    1<<F1MS   ;;; FLAG RESET
  RCALL SW_MAIN
  SBRC  SW_DN, SW
  RJMP  LED_ON
  SBRC  SW_UP, SW
  RJMP  LED_OFF
MAIN9:
	RJMP MAIN

led_on:
  SBI PORTB, LED
  rjmp MAIN9

LED_OFF:
  CBI PORTB, LED
  rjmp MAIN9



SW_MAIN:
  IN    SV_SREG,  SREG    ; ステータスレジスタ保存
  CLR   SW_DN            ; 立ち下がりフラグ・リセット
  CLR   SW_UP            ; 立ち上がりフラグ・リセット
  INC   SW_CNT            ; １０ミリ秒カウンタ・インクリメント
  LDI   TMP, 10           ; 比較用レジスタセット
  CP    SW_CNT, TMP       ; １０カウント？
  BRNE  SW_END            ; １０回に達していなければ抜ける
  CLR   SW_CNT            ; カウンター・リセット

  IN TMP,  PINB        ; スイッチの状態を入力
  ANDI  TMP,  (1<<SW)     ; 不要ビット削除
  MOV   SW_NOW, TMP       ; 今回値を保存
  OR    TMP,  SW_ZZZZ      ; (今回値 OR 前々々回値 OR 前々回値 & 前回値) &  確定値 ; 追加
  OR    TMP,  SW_ZZZ      ; (今回値 OR 前々々回値 OR 前々回値 & 前回値) &  確定値 ; 追加
  OR    TMP,  SW_ZZ      ; 上の続き
  OR    TMP,  SW_Z      ; 上の続き
  AND   TMP,  SW_KAK      ; 上の続き
  MOV   SW_TMP, TMP       ; 計算した途中計算結果を保存
  MOV   TMP,  SW_NOW      ; 今回値をコピー
  AND   TMP,  SW_ZZZZ      ; (今回値 AND 前々々回値 AND 前々回値 AND 前回値) OR (上の途中結果) ; 追加
  AND   TMP,  SW_ZZZ      ; (今回値 AND 前々々回値 AND 前々回値 AND 前回値) OR (上の途中結果) ; 追加
  AND   TMP,  SW_ZZ      ; 上の続き
  AND   TMP,  SW_Z      ; 上の続き
  OR    TMP, SW_TMP       ; 計算完了：新確定値
  MOV   SW_TMP, SW_KAK    ; 前の確定値保存
  MOV   SW_KAK, TMP       ; 計算した新確定値を保存
  MOV   TMP,  SW_TMP      ; 立ち上がり＝〜前確定値 & 新確定値
  COM   TMP               ; 上の続き
  AND   TMP,  SW_KAK      ; 上の続き
  MOV   SW_UP, TMP       ; 立ち上がりフラグ保存
  MOV   TMP,  SW_KAK      ; 立ち下がり＝前確定値 & 〜新確定値
  COM   TMP               ; 上の続き
  AND   SW_TMP, TMP       ; 上の続き
  MOV   SW_DN, SW_TMP    ; 立ち下がりフラグ保存
  MOV   SW_ZZZZ, SW_ZZZ    ; 前々々回 <= 前々回
  MOV   SW_ZZZ, SW_ZZ    ; 前々々回 <= 前々回
  MOV   SW_ZZ, SW_Z    ; 前々回 <= 前回
  MOV   SW_Z, SW_NOW    ; 前回   <= 今回
SW_END:
  OUT   SREG,   SV_SREG   ; シフトレジスタ戻す
  RET


;----------------------------------------------------
TIM0_COMPA_ISR:         ;タマI/カウンタ0比較A一致
  IN  SV_SREG, SREG
	SBR FLAG, 1<<F1MS
  OUT SREG, SV_SREG
	RETI
;----------------------------------------------------
