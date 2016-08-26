#include <stdlib.h>
#include <avr/io.h>
#include "game.h"
#include "arb.h"
#include "shift.h"

#define MAX_GUESSABLE_DIGITS    5
#define ALL_ONES    0b1111111111111111
#define ALL_ZEROES  0

uint8_t _numGuessableDigits;
uint8_t _hasDoneAnything = 0;
uint8_t _hasWon;
uint8_t _numToGuess[MAX_GUESSABLE_DIGITS];
uint8_t _numButtonsPressed;
uint8_t _theButtonsPressed[MAX_GUESSABLE_DIGITS];
uint8_t _numCorrect;
uint8_t _numOrdered;

void startOver();
void startNewTurn();
void generateNewGuessable();
void generateResults();
void showResults();
void showNumDigitsInGuessable();
void displayStatus(uint8_t numButtons, uint8_t numCorrect, uint8_t numOrdered);
void showWin();
uint8_t computeNumCorrect();
uint8_t computeNumOrdered();
uint16_t intToUnary(uint8_t num);
uint8_t randOneToNine();
void previewGuessable();
void outputToLeds(uint16_t toOutput);

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

void onSelectNumGuessableDigits(uint8_t numGuessableDigits) {
    _numGuessableDigits = numGuessableDigits;
    showNumDigitsInGuessable();
}

void onButton(uint8_t button) {
    // if this is the first action, then start a new game
    if(!_hasDoneAnything) {
        _hasDoneAnything = 1;
        startOver();
    }

    // check to see if we need to start a new game (i.e. the player won on the last play)
    if(_hasWon) startOver();

    // the last button pressed got us up to the number of digits to guess
    if(_numButtonsPressed == _numGuessableDigits) startNewTurn();

    // register the new button press

    _numButtonsPressed++;
    _theButtonsPressed[_numButtonsPressed-1] = button;

    // now generate and show the results

    generateResults();
    showResults();
}

void outputToLeds(uint16_t toOutput) {
    // NOTES:
    // o srPutBytes will load the least significant bit first, so it will be the "right-most" bit.
    // o the first bit (right-most) doesn't have an led connected.

    srPutBytes(toOutput << 1);
}

void generateResults() {
    // only generate results when the number of guessable digits have been pressed
    if(_numButtonsPressed < _numGuessableDigits) return;

    _numCorrect = computeNumCorrect();
    _numOrdered = computeNumOrdered();

    // if the player won
    if(_numOrdered == _numGuessableDigits) _hasWon = 1;
}

void showResults() {
    // the player won
    if(_hasWon) showWin();
    // if we haven't gotten enough button pushes, then just show the number of button pushes
    else if(_numButtonsPressed < _numGuessableDigits) displayStatus(_numButtonsPressed, 0, 0);
    // otherwise, show the num correct and ordered (don't show num pressed, since it's redundant and confusing)
    else displayStatus(0, _numCorrect, _numOrdered);
}

void displayStatus(uint8_t numButtons, uint8_t numCorrect, uint8_t numOrdered) {
    uint16_t toDisplay = 0
        // show these on the right-most five leds
        | (intToUnary(numButtons))
        // show on the middle five leds
        | (intToUnary(numOrdered) << 5)
        // show on the left-most five leds
        | (intToUnary(numCorrect) << 10);

    outputToLeds(toDisplay);
}

// will take a number like 3, and convert it to a unary number (e.g. 0b0000000000000111).
// assumes number will be between 0 and 16
uint16_t intToUnary(uint8_t num) {
    switch(num) {
        case 16: return 0b1111111111111111;
        case 15: return 0b0111111111111111;
        case 14: return 0b0011111111111111;
        case 13: return 0b0001111111111111;
        case 12: return 0b0000111111111111;
        case 11: return 0b0000011111111111;
        case 10: return 0b0000001111111111;
        case 9:  return 0b0000000111111111;
        case 8:  return 0b0000000011111111;
        case 7:  return 0b0000000001111111;
        case 6:  return 0b0000000000111111;
        case 5:  return 0b0000000000011111;
        case 4:  return 0b0000000000001111;
        case 3:  return 0b0000000000000111;
        case 2:  return 0b0000000000000011;
        case 1:  return 0b0000000000000001;
        case 0:
        default: return 0b0000000000000000;
    }
}

void showWin() {
    uint8_t i;

    for(i=0; i < 10; i++) {
        outputToLeds(ALL_ONES);
        delayMsish(200);
        outputToLeds(ALL_ZEROES);
        delayMsish(200);
    }
}

void onGameStartup() {
    uint8_t steps[] = {
        0b00000100,
        0b00001010,
        0b00010001,
        0b00001010,
        0b00000100,
        0b00001110,
        0b00011111,
        0b00000000,
        0b00011111,
        0b00000000,
        0b00011111,
        0b00000000,
        0b00011111,
        0b00001110,
        0b00001110,
        0b00000100,
    };

    uint8_t l = sizeof(steps)/sizeof(uint8_t);
    uint8_t i;

    for(i=0; i < l; i++) {
        displayStatus(steps[i], steps[i], steps[i]);
        delayMsish(200);
    }

    outputToLeds(ALL_ZEROES);
}

void showNumDigitsInGuessable() {
    uint8_t i;

    for(i=0; i < 5; i++) {
        outputToLeds(intToUnary(_numGuessableDigits));
        delayMsish(200);
        outputToLeds(ALL_ZEROES);
        delayMsish(200);
    }
}

uint8_t computeNumCorrect() {
    uint8_t i;
    uint8_t j;
    uint8_t numCorrect = 0;

    // copy the buttons pressed, so we can change it as we find numbers
    uint8_t buttonsPressed[MAX_GUESSABLE_DIGITS];
    for(i=0; i < MAX_GUESSABLE_DIGITS; i++) buttonsPressed[i] = _theButtonsPressed[i];

    // go through the number to guess, see how many are correct
    for(i=0; i < _numGuessableDigits; i++) {
        for(j=0; j < _numGuessableDigits; j++) {
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

    for(i = 0; i < _numGuessableDigits; i++) {
        if(_numToGuess[i] == _theButtonsPressed[i]) numOrdered++;
    }

    return numOrdered;
}

// generates a new _numGuessableDigits digit number to guess (without any zeroes in it)
void generateNewGuessable() {
    srand(arbc());

    uint8_t i;
    
    for(i=0; i < _numGuessableDigits; i++) _numToGuess[i] = randOneToNine();

    previewGuessable();//stub
}

void previewGuessable() {
    uint8_t i;

    for(i=0; i < _numGuessableDigits; i++) {
        outputToLeds(intToUnary(_numToGuess[i]));
        delayMsish(1500);
    }

    outputToLeds(ALL_ZEROES);
}

uint8_t randOneToNine() {
    // %9 gives a number between 0-8. +1 gives a numbet between 1-9.
    return (rand() % 9) + 1;
}
