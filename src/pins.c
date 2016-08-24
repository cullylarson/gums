#include <avr/io.h>
#include "pins.h"

void setupPins() {
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
    
    // start high so that reading a 0 means pressed?
    B1_PORT |= (1 << B1);
    B2_PORT |= (1 << B2);
}
