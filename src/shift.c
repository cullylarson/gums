#include "pins.h"
#include "shift.h"

void srPutBit(uint8_t bit);
void srLatch();
void srCycleClock();

/*
 * Writes two bytes out serially to SR_SER
 */
void srPutBytes(uint16_t toPut) {
    uint8_t i;
    for(i = 0; i < 16; i++) {
        srPutBit(toPut & 1);
        toPut >>= 1;
    }

    srLatch();
}


/*
 * Writes a bit out serially to SR_SER
 */
void srPutBit(uint8_t bit) {
    if(bit == 0) GOLO(SR_SER_PORT, SR_SER);
    else         GOHI(SR_SER_PORT, SR_SER);

    srCycleClock();
}

/*
 * Tells the shift register to output all the bits it's read
 */
void srLatch() {
    GOHI(SR_RCLK_PORT, SR_RCLK);
    GOLO(SR_RCLK_PORT, SR_RCLK);
}

/*
 * Tells the shift register to read a bit
 */
void srCycleClock() {
    GOHI(SR_SRCLK_PORT, SR_SRCLK);
    GOLO(SR_SRCLK_PORT, SR_SRCLK);
}
