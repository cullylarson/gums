#include <avr/io.h>
#include <avr/interrupt.h>
#include "pins.h"
#include "arb.h"
#include "buttons.h"

void setup();

int main(void) {
    setup();

    uint8_t b;
    uint8_t b1_on = 0;
    uint8_t b2_on = 0;

    while(1) {
        b = getPressedButton();

        /*
        if(readButton(1))  GOHI(PORTB, LED1);
        else        GOLO(PORTB, LED1);

        if(readButton(2))  GOHI(PORTD, LED2);
        else        GOLO(PORTD, LED2);
        */

        if(b == 1) {
            if(b1_on) {
                b1_on = 0;
                GOLO(PORTB, LED1);
            }
            else {
                b1_on = 1;
                GOHI(PORTB, LED1);
            }
        }

        if(b == 2) {
            if(b2_on) {
                b2_on = 0;
                GOLO(PORTD, LED2);
            }
            else {
                b2_on = 1;
                GOHI(PORTD, LED2);
            }
        }
    }
}

void setup() {
    setupArb();
    setupPins();

    sei();
}
