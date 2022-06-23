#include <Arduino.h>

#define TEMP A1
#define POTENTIOMETER A0
#define LED 13
#define ON_BUTTON 12
#define OFF_BUTTON 11
#define DONE 10
#define MAX_TEMP 100

int maximumTemperature = 0;
int currentTemperature = 0;
bool isRunning = false;
bool isDone = false;

/**
* Setup for the Arduino
*/
void setup() {
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    pinMode(ON_BUTTON, INPUT);
    pinMode(OFF_BUTTON, INPUT);
    pinMode(DONE, OUTPUT);
}

/**
* The loop that is constantly triggered
*/
void loop() {
    checkOffButton();
    checkOnButton();
    setMaximumTemperature();
    setActualTemperature();
    toggleHeater();
}

/**
* Sets the running boolean to true if not running and on button is pressed
*/
void checkOnButton() {
    const int buttonValue = digitalRead(ON_BUTTON);
    if (!isRunning && buttonValue == HIGH) {
        isRunning = true;
        digitalWrite(DONE, LOW);
    }
}

/**
* Sets the running boolean to false if it is running and the off button is pressed
*/
void checkOffButton() {
    const int buttonValue = digitalRead(OFF_BUTTON);
    if (isRunning && buttonValue == HIGH) {
        isRunning = false;
        digitalWrite(LED, LOW);
        Serial.println("Canceled the heating");
    }
}

/**
* Checks if the heater has to be turned on or off and toggles it thereafter
*/
void toggleHeater() {
    if (isRunning) {
        if (currentTemperature >= maximumTemperature) {
            Serial.println("Max Temp has been reached");
            isRunning = false;
            digitalWrite(DONE, HIGH);
            digitalWrite(LED, LOW);
        } else {
            digitalWrite(LED, HIGH);
        }
    }
}

/**
* fetches the current temperature from the temperature sensor
* read from the port set by TEMP
*/
void setActualTemperature() {
    const int tempRead = analogRead(TEMP);
    //map the input from the sensor to a value in degree celsius
    currentTemperature = map(tempRead, 0, 410, -50, 150);
}

/**
* Measures the maximum temperature by the power returned
* by the potentiometer on the port set by POTENTIOMETER
*/
void setMaximumTemperature() {

    const int oldMax = maximumTemperature;

    const int potentiometerRead = analogRead(POTENTIOMETER);
    const double part = (double) potentiometerRead / (double) 675;
    maximumTemperature = part * MAX_TEMP;

    //print a message about the new max to inform the user
    if (oldMax != maximumTemperature) {
        Serial.print("New Max: ");
        Serial.println(maximumTemperature);
    }
}