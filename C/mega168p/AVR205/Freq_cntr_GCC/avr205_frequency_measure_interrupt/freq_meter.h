void freq_cntr_init(void);

void freq_cntr_start_measurement(void);

unsigned int freq_cntr_get_result(void);

void freq_cntr_clear_result(void);

unsigned int freq_cntr_get_frequency(void);

void ovf_test_and_return_result(void);


#ifndef F_CPU
#define F_CPU 8000000UL
#endif


#define FREQ_CNTR_CTRLA			TCCR1A
#define FREQ_CNTR_CTRLB			TCCR1B
#define GATE_CNTR_CTRLB			TCCR0B
#define TC_COUNTMODE			TCW0
#define PIN_CHG_MASK			PCMSK2
#define PIN_CHG_INT				PCINT21
#define GEN_INT_MASK			PCICR
#define PIN_CHG_INT_ENAB		PCIE2
#define PIN						PIND
#define PIN_NO					PD5
#define FREQ_CNTR_INT_FLAG_REG	TIFR1
#define	FREQ_CNTR_OVF_FLAG		TOV1
#define GATE_CNTR_INT_FLAG_REG	TIFR0
#define	GATE_CNTR_OVF_FLAG		TOV0
#define	FREQ_CNTR_COUNT_LOW		TCNT1L
#define	FREQ_CNTR_COUNT_HIGH	TCNT1H
#define	GATE_CNTR_COUNT			TCNT0
#define	GATE_CNTR_PRELOAD		255-195
#define GATE_CNTR_INT_MASK_REG	TIMSK0
#define GATE_CNTR_OVF_INT_ENAB	TOIE0
#define CLOCK_SEL_00			CS00
#define CLOCK_SEL_02			CS02
#define CLOCK_SEL_1				CS11
#define CLOCK_SEL_2				CS12
#define CLOCK_SEL_10			CS10
#define CLOCK_SEL_12			CS12
#define CLOCK_SEL_13			CS13
#define GEN_INT_FLG_REG			GIFR
#define PIN_CHG_INT_FLAG		PCIF
#define GATE_CNTR_OVF_vect		TIMER0_OVF_vect
#define PIN_CHG_vect			PCINT2_vect
