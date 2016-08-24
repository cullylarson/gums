#include <avr/io.h>

#ifndef PINS_H
#define PINS_H

#define LED1        PB0
#define LED2        PD7

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

// if that bit is zero, the switch is being pressed. if it's 1, not being pressed
#define READ(port, pn) (port & (1<<pn)) == 0

void setupPins();

#endif
