#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "pins.h"
#include "arb.h"
#include "buttons.h"
#include "shift.h"
#include "arb.h"

#define ALL_ONES    0b1111111111111111
#define ALL_ZEROES  0

uint8_t _hasDoneAnything = 0;
uint8_t _hasWon;
uint16_t _numToGuess[4];
uint16_t _numButtonsPressed;
uint8_t _theButtonsPressed[4];
uint8_t _numCorrect;
uint8_t _numOrdered;

void setup();
void startOver();
void startNewTurn();
void generateNewGuessable();
void onButton(uint8_t button);
void generateResults();
void showResults();
void displayResults(uint8_t numButtons, uint16_t numCorrect, uint16_t numOrdered);
void showWin();
uint8_t computeNumCorrect();
uint8_t computeNumOrdered();
uint8_t intToUnary(uint8_t num);
uint8_t randOneToNine();

int main(void) {
    setup();

    uint8_t b;

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

void startOver() {
    generateNewGuessable();
    startNewTurn();
}

void startNewTurn() {
    _hasWon = 0;
    _numButtonsPressed = 0;
    _numCorrect = 0;
    _numOrdered = 0;

    // no need to zero out _theButtonsPressed, since each press will overwrite the old one
    // and results aren't displayed until all values are set again.
}

void onButton(uint8_t button) {
    // if this is the first action, then start a new game
    if(!_hasDoneAnything) {
        _hasDoneAnything = 1;
        startOver();
    }

    // check to see if we need to start a new game (i.e. the player won on the last play)
    if(_hasWon) startOver();

    // the last button pressed got us up to four
    if(_numButtonsPressed == 4) startNewTurn();

    // register the new button press

    _numButtonsPressed++;
    _theButtonsPressed[_numButtonsPressed-1] = button;

    // now generate and show the results

    generateResults();
    showResults();
}

void generateResults() {
    // only generate results when four numbers have been pressed
    if(_numButtonsPressed < 4) return;

    _numCorrect = computeNumCorrect();
    _numOrdered = computeNumOrdered();

    // if the player won
    if(_numOrdered == 4) _hasWon = 1;
}

void showResults() {
    // the player won
    if(_hasWon) showWin();
    // if we haven't gotten enough button pushes, then just show the number of button pushes
    else if(_numButtonsPressed < 4) displayResults(_numButtonsPressed, 0, 0);
    // otherwise, show everything
    else displayResults(_numButtonsPressed, _numCorrect, _numOrdered);
}

void displayResults(uint8_t numButtons, uint16_t numCorrect, uint16_t numOrdered) {
    // NOTES:
    // o srPutBytes will load the least significant bit first.
    // o the first four bits don't have any leds connected.

    uint16_t toDisplay = 0
        // show these on the right-most four leds
        | (intToUnary(numButtons) << 4)
        // show on the middle four leds
        | (intToUnary(numCorrect) << 8)
        // show on the left-most four leds
        | (intToUnary(numOrdered) << 12);

    srPutBytes(toDisplay);
}

// will take a number like 3, and convert it to a unary number (e.g. 0b00000111).
// assumes number will be between 0 and 4
uint8_t intToUnary(uint8_t num) {
    switch(num) {
        case 4: return 0b00001111;
        case 3: return 0b00000111;
        case 2: return 0b00000011;
        case 1: return 0b00000001;
        case 0:
        default: return 0b00000000;
    }
}

void showWin() {
    uint8_t i;

    for(i=0; i < 10; i++) {
        srPutBytes(ALL_ONES);
        delayMsish(200);
        srPutBytes(ALL_ZEROES);
        delayMsish(200);
    }
}

uint8_t computeNumCorrect() {
    uint8_t i;
    uint8_t j;
    uint8_t numCorrect = 0;

    // copy the buttons pressed, so we can change it as we find numbers
    uint8_t buttonsPressed[4];
    for(i=0; i < 4; i++) buttonsPressed[i] = _theButtonsPressed[i];

    // go through the number to guess, see how many are correct
    for(i=0; i < 4; i++) {
        for(j=0; j < 4; j++) {
            // we found a match
            if(_numToGuess[i] == buttonsPressed[j]) {
                numCorrect++;
                // unset the value we found, so it isn't matched again
                buttonsPressed[j] = 0;
                // now end this loop, so that this value in the number to guess doesn't match another value in our buttonsPressed
                break;
            }
        }
    }

    return numCorrect;
}

uint8_t computeNumOrdered() {
    uint8_t i;
    uint8_t numOrdered = 0;

    for(i = 0; i < 4; i++) {
        if(_numToGuess[i] == _theButtonsPressed[i]) numOrdered++;
    }

    return numOrdered;
}

// returns a four digit number without any zeroes in it
void generateNewGuessable() {
    srand(arbc());

    _numToGuess[0] = randOneToNine();
    _numToGuess[1] = randOneToNine();
    _numToGuess[2] = randOneToNine();
    _numToGuess[3] = randOneToNine();
}

uint8_t randOneToNine() {
    // %9 gives a number between 0-8. +1 gives a numbet between 1-9.
    return (rand() % 9) + 1;
}
