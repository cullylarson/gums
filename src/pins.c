#include <avr/io.h>
#include "pins.h"

void setupPins() {
    // set output pins
    SR_SER_DDR   |= (1 << SR_SER);
    SR_SRCLK_DDR |= (1 << SR_SRCLK);
    SR_RCLK_DDR  |= (1 << SR_RCLK);

    // set input pins
    B1_DDR &= ~(1 << B1);
    B2_DDR &= ~(1 << B2);
    B3_DDR &= ~(1 << B3);
    B4_DDR &= ~(1 << B4);
    B5_DDR &= ~(1 << B5);
    B6_DDR &= ~(1 << B6);
    B7_DDR &= ~(1 << B7);
    B8_DDR &= ~(1 << B8);
    B9_DDR &= ~(1 << B9);
    
    // start high so that reading a 0 means pressed?
    B1_PORT |= (1 << B1);
    B2_PORT |= (1 << B2);
    B3_PORT |= (1 << B3);
    B4_PORT |= (1 << B4);
    B5_PORT |= (1 << B5);
    B6_PORT |= (1 << B6);
    B7_PORT |= (1 << B7);
    B8_PORT |= (1 << B8);
    B9_PORT |= (1 << B9);
}
