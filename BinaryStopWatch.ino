#include <Arduino.h>

int year = 0;
long week = 0;
long day = 0;
long hour = 0;
long minute = 0;
long second = 0;
long milliSecond = 0;

long startTime = 0;
long stopTime = 0;
bool isRunning = false;

void setup() {
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    Serial.begin(9800);
}

void loop() {
    checkButtons();
    if (isRunning) {
        reloadTime();
        printTime();
        displayMinutes();
        displaySeconds();
        delay(100);
    } else {
        delay(50);
    }
}

/**
 * Check if the buttons are pressed and updates the time and stats thereafter
 */
void checkButtons() {
    if (!isRunning && digitalRead(A0) == HIGH) {
        isRunning = true;
        startTime = millis();
    } else if (isRunning && digitalRead(A1) == HIGH) {
        isRunning = false;
        stopTime = millis();
    } else if (!isRunning && startTime > 0 && digitalRead(A2) == HIGH) {
        isRunning = true;
        const int sinceStop = (millis() - stopTime);
        startTime = startTime + sinceStop;
    } else if (digitalRead(A3) == HIGH) {
        startTime = millis();
    }
}

/**
 * Reloads the time from the current time
 */
void reloadTime() {
    const long sinceEpoch = millis() - startTime;

    second = sinceEpoch / 1000;
    milliSecond = sinceEpoch - (second * 1000);

    minute = second / 60;
    second = second - (minute * 60);

    hour = minute / 60;
    minute = minute - (hour * 60);
}

/**
 * Prints the time to the serial port
 */
void printTime() {
    Serial.print(year);
    Serial.print("-");
    Serial.print(day);
    Serial.print(" ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(":");
    Serial.print(second);
    Serial.print(".");
    Serial.println(milliSecond);
}

/**
 * Displays the minutes on the binary display
 */
void displayMinutes() {
    int values[6];
    fillFromValue(values, minute, 6);
    setOutputs(values, 8, 6);
}

/**
 * Displays the seconds on the binary display
 */
void displaySeconds() {
    int values[6];
    fillFromValue(values, second, 6);
    setOutputs(values, 2, 6);
}

/**
 * Sets the outputs to the given values in the given array
 * @param toSet array containing the values to set
 * @param offset the offset to start setting the values at(port)
 * @param size the size of the array
 */
void setOutputs(int toSet[], int offset, int size) {
    for (int i = 0; i < size; i++) {
        digitalWrite(i + offset, toSet[i]);
    }
}

/**
 * fills the array with the binary representation of the given value
 * @param toFill the array to fill
 * @param value the value to fill the array with
 * @param size the size of the array
 */
void fillFromValue(int toFill[], long value, int size) {
    //filling the array acordingly
    for (int i = 0; i < size; i++) {
        if (value > 0) {
            toFill[i] = value % 2;
            value = value / 2;
        } else {
            toFill[i] = 0;
        }
    }
}