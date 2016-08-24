#include "pins.h"
#include "buttons.h"
#include "arb.h"

#define DEBOUNCE_WAIT 10

uint8_t retWhicheverPressed(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7, uint8_t b8, uint8_t b9);
uint8_t getButtonDown();
void waitForButtonUp(uint8_t buttonDown);

uint8_t readButton(uint8_t button) {
    switch(button) {
        case 1: return READ(B1_IN, B1);
        case 2: return READ(B2_IN, B2);
        default: return 0;
    }
}

// reads the present, rightnow, momentary, transient value of all the
// buttons and returns whichever is pressed. 0 means no button pressed.
uint8_t readPressedButtonNow() {
    uint8_t b1 = readButton(1);
    uint8_t b2 = readButton(2);
    uint8_t b3 = readButton(3);
    uint8_t b4 = readButton(4);
    uint8_t b5 = readButton(5);
    uint8_t b6 = readButton(6);
    uint8_t b7 = readButton(7);
    uint8_t b8 = readButton(8);
    uint8_t b9 = readButton(9);

    return retWhicheverPressed(b1, b2, b3, b4, b5, b6, b7, b8, b9);
}

// finds out which of the 9 buttons is pressed. 0 means no button pressed.
// does debouncing, to make sure the button was pressed.  will return on
// button up.
uint8_t getPressedButton() {
    uint8_t buttonDown = getButtonDown();
    waitForButtonUp(buttonDown);
    return buttonDown;
}

// finds out which button is currently down. does debounce.
uint8_t getButtonDown() {
    uint8_t lastButton = 255;
    uint8_t count = 0;
    uint8_t pressed;

    // consider a button down if it's down for four counts
    while(count <= 4) {
        pressed = readPressedButtonNow();

        if(pressed == lastButton) {
            count++;
        }
        else {
            lastButton = pressed;
            count = 0;
        }

        delayMsish(DEBOUNCE_WAIT);
    }

    return lastButton;
}

// waits for a button to come up (assuming it's already down). does debounce.
void waitForButtonUp(uint8_t buttonDown) {
    uint8_t count = 0;
    uint8_t pressed;

    // if the button is 0, then return 0 because it probably won't change
    if(buttonDown == 0) return 0;

    // now wait for the button to change state
    while(count <= 4) {
        pressed = readPressedButtonNow();

        // button is different
        if(pressed != buttonDown) {
            count++;
        }
        // otherwise it's the same
        else {
            count = 0;
        }

        delayMsish(DEBOUNCE_WAIT);
    }
}

// takes a set of button pressed values (1 or 0) and returns the first
// that is pressed (1). will return 0 if none pressed.
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
