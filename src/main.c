#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "pins.h"
#include "shift.h"
#include "buttons.h"
#include "arb.h"
#include "game.h"

void setup();

int main(void) {
    setup();

    // start with leds empty
    srPutBytes(0);

    onGameStartup();

    uint8_t b;
    uint8_t haveSelectedNumGuessableDigits = 0;

    // get the number of selectable digits
    while(!haveSelectedNumGuessableDigits) {
        b = getPressedButton();

        // only allow 3 to 5 digits
        if(b >= 3 && b <= 5) {
            haveSelectedNumGuessableDigits = 1;
            onSelectNumGuessableDigits(b);
        }
    }

    while(1) {
        b = getPressedButton();

        if(b == 0) continue;
        else onButton(b);
    }
}

void setup() {
    setupArb();
    setupPins();

    sei();
}
