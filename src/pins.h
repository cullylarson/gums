#include <avr/io.h>

#ifndef PINS_H
#define PINS_H

#define SR_SER          PD0
#define SR_SER_DDR      DDRD
#define SR_SER_PORT     PORTD

#define SR_RCLK         PD1
#define SR_RCLK_DDR     DDRD
#define SR_RCLK_PORT    PORTD

#define SR_SRCLK        PD2
#define SR_SRCLK_DDR    DDRD
#define SR_SRCLK_PORT   PORTD

#define B1          PB1
#define B1_PORT     PORTB
#define B1_DDR      DDRB
#define B1_IN       PINB

#define B2          PB2
#define B2_PORT     PORTB
#define B2_DDR      DDRB
#define B2_IN       PINB

#define B3          PC0
#define B3_PORT     PORTC
#define B3_DDR      DDRC
#define B3_IN       PINC

#define B4          PC1
#define B4_PORT     PORTC
#define B4_DDR      DDRC
#define B4_IN       PINC

#define B5          PC2
#define B5_PORT     PORTC
#define B5_DDR      DDRC
#define B5_IN       PINC

#define B6          PC3
#define B6_PORT     PORTC
#define B6_DDR      DDRC
#define B6_IN       PINC

#define B7          PC4
#define B7_PORT     PORTC
#define B7_DDR      DDRC
#define B7_IN       PINC

#define B8          PC5
#define B8_PORT     PORTC
#define B8_DDR      DDRC
#define B8_IN       PINC

#define B9          PB0
#define B9_PORT     PORTB
#define B9_DDR      DDRB
#define B9_IN       PINB

// write digital "high" to pin <pn> on port <port>
#define GOHI(port, pn) port |= (1<<pn)

// write digital "low" to pin <pn> on port <port>
#define GOLO(port, pn) port &= ~(1<<pn)

// if that bit is zero, the switch is being pressed. if it's 1, not being pressed
#define READ(port, pn) (port & (1<<pn)) == 0

void setupPins();

#endif
