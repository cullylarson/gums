#include <avr/io.h>
#include <avr/interrupt.h>
#include "pins.h"
#include "arb.h"
#include "buttons.h"
#include "shift.h"

void setup();

int main(void) {
    setup();

    uint8_t b;

    uint16_t leds = 0;

    srPutBytes(leds);

    while(1) {
        b = getPressedButton();

        if(b == 0) continue;
        else {
            leds ^= 1 << (b+3);
            srPutBytes(leds);
        }
    }
}

void setup() {
    setupArb();
    setupPins();

    sei();
}
