#include <avr/io.h>
#include <avr/interrupt.h>
#include "arb.h"
#include "pins.h" // stub

volatile uint32_t _arbitraryCounts = 0;
volatile uint8_t _on = 0;

void setupArb() {
    // Setup Timer0 to fire every 1ms (ish)
    TCNT0  = 254;         // Reset Timer Count to 254 out of 255
    TIFR0  = 0x00;        // Timer0 INT Flag Reg: Clear Timer Overflow Flag
    TIMSK0 = 0x01;        // Timer0 INT Reg: Timer2 Overflow Interrupt Enable
    TCCR0B = 0x02;        // Timer0 Control Reg B: Timer Prescaler set to 8
}

void delayMsish(uint16_t msish) {
    delayArb((uint32_t) msish * 100);
}

void delayArb(uint32_t c) {
    uint32_t calledAt = arbc();
    uint32_t diff = 0;

    while(diff < c) {
        diff = arbc() - calledAt;
    }
}

// get arbitrary counts
uint32_t arbc() {
    return _arbitraryCounts;
}

// called occasionally (possibly around every 1ms)
ISR(TIMER0_OVF_vect) {
	_arbitraryCounts++;

    TCNT0  = 254;     // reset counter
    TIFR0  = 0x00;    // clear timer overflow flag
};
