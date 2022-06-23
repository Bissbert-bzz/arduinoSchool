/*
 * @Author Bissbert
 * Reason for this project:
 * I wanted to create a party led display that can be set to a few different patters.
 * These different states could A) be used to indicate a state or B) to just have some fun party lights :)
 */
#include <Arduino.h>

#define WAIT_BETWEEN 1000

int ledArray[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
bool *ledActive;
int ledCount = 0;

long toggleTime = -1;

int singleCurrentLed = 0;
int singleLastLed = -1;

int currentPattern = 0;

void setup() {
    ledCount = sizeof(ledArray) / sizeof(ledArray[0]);
    ledActive = new bool[ledCount];
    for (int i = 0; i < ledCount; i++) {
        pinMode(ledArray[i], OUTPUT);
    }
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
}

void loop() {
    checkButtons();
    if (timePassedSince(toggleTime, WAIT_BETWEEN)) {
        triggerPattern();
    }
}

/**
 * Check if the buttons are pressed and updates the patter thereafter
 */
void checkButtons() {
    if (digitalRead(A5) == HIGH) {
        currentPattern = 1;
        clearLed();
    } else if (digitalRead(A4) == HIGH) {
        currentPattern = 2;
        clearLed();
    } else if (digitalRead(A3) == HIGH) {
        currentPattern = 0;
        clearLed();
    }
}

/**
 * clears the led
 */
void clearLed() {
    singleCurrentLed = 0;
    singleLastLed = -1;
    for (int i = 0; i < ledCount; i++) {
        ledActive[i] = false;
        digitalWrite(ledArray[i], LOW);
    }
}

/**
 * triggers the currently selected pattern
 */
void triggerPattern() {
    if (currentPattern == 1) {
        toggleRow();
    } else if (currentPattern == 2) {
        toggleRow(singleCurrentLed, singleLastLed, 3);
    } else {
        doubleTrigger();
    }
}

bool isFirst = true;

/**
 * triggers the double pattern
 */
void doubleTrigger() {
    if (isFirst) {
        toggleRow(singleCurrentLed, singleLastLed, 2);
    } else {
        toggleRow(singleCurrentLed, singleLastLed, -1);
    }
    isFirst = !isFirst;
}

/**
 * triggers the row pattern
 */
void toggleRow() {
    toggleRow(singleCurrentLed, singleLastLed, 1);
}

/**
 * triggers the row pattern with the given parameters
 * @param current the current led to turn on
 * @param last the last led to turn off
 * @param step increment of the led
 */
void toggleRow(int &current, int &last, int step) {
    if (last >= 0) {
        toggleLedWithNext(last, step);
    }
    last = toggleLedWithNext(current, step);
}

/**
 * toggles the led with the given index and increments the index by the given step
 * @param id the index of the led to toggle
 * @param step the increment of the index
 * @return the index of the led that was toggled
 */
int toggleLedWithNext(int &id, int step) {
    const int current = id;
    toggleLed(id);
    if (id + step >= ledCount) {
        id = 0;
    } else {
        id += step;
    }
    return current;
}

/**
 * toggles the led with the given index
 * @param id the index of the led to toggle
 */
void toggleLed(int id) {
    ledActive[id] = !ledActive[id];
    if (ledActive[id]) {
        digitalWrite(ledArray[id], HIGH);
    } else {
        digitalWrite(ledArray[id], LOW);
    }
}

/**
 * checks if a given delay has happened since the passed time and
 * updates the variable if it did
 * @param ref reference to the variable containing the current time
 * @param delay the delay to check
 * @return true if the delay has happened else false
 */
bool timePassedSince(long &ref, long delay) {
    const bool hasPassed = millis() - ref >= delay;
    if (hasPassed) {
        ref = millis();
    }
    return hasPassed;
}