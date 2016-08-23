#include <avr/io.h>
#include <avr/interrupt.h>

#define DEBOUNCE_ARBC  500000
#define LED1      PB0
#define LED2      PD7

#define B1          PB1
#define B1_PORT     PORTB
#define B1_DDR      DDRB
#define B1_IN       PINB

#define B2          PB2
#define B2_PORT     PORTB
#define B2_DDR      DDRB
#define B2_IN       PINB

// write digital "high" to pin <pn> on port <port>
#define GOHI(port, pn) port |= (1<<pn)

// write digital "low" to pin <pn> on port <port>
#define GOLO(port, pn) port &= ~(1<<pn)

#define READ(port, pn) port & (1<<pn)

volatile uint32_t _arbitraryCounts = 0;
volatile uint32_t _debounceLastCount = 0;
volatile uint8_t _debounceLastButton = 255;


//volatile uint8_t _on = 0;

void setup();
uint32_t arbc();
void delayArb(uint32_t c);
void delayMsish(uint16_t msish);
uint8_t readButton(uint8_t button);
uint8_t getPressedButton();
uint8_t retWhicheverPressed(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7, uint8_t b8, uint8_t b9);

int main(void) {
    setup();

    uint8_t b = 0;

    while(1) {
        /*
        if(readButton(1)) {
            GOHI(PORTB, LED1);
        }
        else{
            GOLO(PORTB, LED1);
        }

        if(readButton(2)) {
            GOHI(PORTD, LED2);
        }
        else {
            GOLO(PORTD, LED2);
        }
        */

        b = getPressedButton();

        if(b == 1)  GOHI(PORTB, LED1);
        else        GOLO(PORTB, LED1);

        if(b == 2)  GOHI(PORTD, LED2);
        else        GOLO(PORTD, LED2);
    }
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

void setup() {
    // set output pins on portd
    DDRD |= (1 << LED2);

    // set output pins on portb
    DDRB |= (1 << LED1);
    // set input pins
    B1_DDR &= ~(1 << B1);
    B2_DDR &= ~(1 << B2);

    // start pins low
    PORTB |= (0 << LED1);
    PORTD |= (0 << LED2);
    B1_PORT |= (0 << B1);
    B2_PORT |= (0 << B2);


    // Setup Timer0 to fire every 1ms
    TCNT0  = 254;         // Reset Timer Count to 254 out of 255
    TIFR0  = 0x00;        // Timer0 INT Flag Reg: Clear Timer Overflow Flag
    TIMSK0 = 0x01;        // Timer0 INT Reg: Timer2 Overflow Interrupt Enable
    TCCR0B = 0x02;        // Timer0 Control Reg B: Timer Prescaler set to 8

    sei();
}

uint8_t readButton(uint8_t button) {
    switch(button) {
        case 1: return READ(B1_IN, B1);
        case 2: return READ(B2_IN, B2);
        default: return 0;
    }
}

// finds out which of the 9 buttons is pressed. 0 means no button pressed
uint8_t getPressedButton() {
    uint8_t b1 = readButton(1);
    uint8_t b2 = readButton(2);
    uint8_t b3 = readButton(3);
    uint8_t b4 = readButton(4);
    uint8_t b5 = readButton(5);
    uint8_t b6 = readButton(6);
    uint8_t b7 = readButton(7);
    uint8_t b8 = readButton(8);
    uint8_t b9 = readButton(9);

    uint8_t pressed = retWhicheverPressed(b1, b2, b3, b4, b5, b6, b7, b8, b9);
    uint32_t now = arbc();
    // this isn't reliable because the counter circles around?
    uint32_t diff = now - _debounceLastCount;

    // if no button last pressed, then consider the current one pressed
    if(_debounceLastButton == 255) {
        _debounceLastButton = pressed;
        _debounceLastCount = now;
        return pressed;
    }

    // if the same button is pressed, then don't count it unless a certain amount of time has passed
    if(_debounceLastButton == pressed && diff < DEBOUNCE_ARBC) {
        _debounceLastCount = now;
        return 0;
    }

    // at this point, we know a button was pressed last and either the button is different or a good amount of time has passed
    _debounceLastButton = pressed;
    return pressed;
}

uint8_t retWhicheverPressed(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7, uint8_t b8, uint8_t b9) {
    if(b1) return 1;
    else if(b2) return 2;
    else if(b3) return 3;
    else if(b4) return 4;
    else if(b5) return 5;
    else if(b6) return 6;
    else if(b7) return 7;
    else if(b8) return 8;
    else if(b9) return 9;
    else return 0;
}

// get arbitrary counts
uint32_t arbc() {
    return _arbitraryCounts;
}

// should be called every 1ms
ISR(TIMER0_OVF_vect) {
	_arbitraryCounts++;

    /*
    if((_arbitraryCounts-1) % 1500 == 0) {
        if(_on) {
            _on = 0;
            GOLO(PORTB, LED1);
        }
        else {
            _on = 1;
            GOHI(PORTB, LED1);
        }
    }
    */

    TCNT0  = 254;     // reset counter
    TIFR0  = 0x00;    // clear timer overflow flag
};
