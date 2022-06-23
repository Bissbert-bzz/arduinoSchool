#include <Arduino.h>

#define RELAYOUT 3
#define ONBUTTON 5
#define SKIPPBUTTON 6
#define ONLAMP 2

bool isOn = false;
bool isSkipping = false;
bool isSkipActive = false;

long toggleTime = -1;
long skipTime = -1;
long skipActiveTime = -1;

void setup() {
    pinMode(RELAYOUT, OUTPUT);
    pinMode(ONLAMP, OUTPUT);
    pinMode(ONBUTTON, INPUT);
    pinMode(SKIPPBUTTON, INPUT);
    Serial.begin(9600);
}

void loop() {
    activate();
    checkToggle();
}

/**
* checks if is on, else sets to off state
*/
void activate() {
    if (isOn) {
        activateOn();
    } else {
        activateOutputs(LOW);
    }
}

/**
* If skipping is active stars skipping script, otherwise sets to on state
*/
void activateOn() {
    if (isSkipping) {
        activateSkipping();
        return;
    }
    activateOutputs(HIGH);
}

/**
* Activates the outputs depending on
*/
void activateSkipping() {
    if (isSkipActive) {
        activateOutputs(HIGH);
    } else {
        activateOutputs(LOW);
    }
    switchSkipActiveState();
}

/**
* Toggles the isActive state every second
*/
void switchSkipActiveState() {
    if (delayBetweenHappened(skipActiveTime, 1000)) {
        isSkipActive = !isSkipActive;
    }
}

/**
* Checks the button inputs and toggles thereafter.
* Each button has a 500ms delay between being pressable
*/
void checkToggle() {
    if (digitalRead(ONBUTTON) == HIGH && delayBetweenHappened(toggleTime, 500)) {
        Serial.println("Toggle Pump to");
        Serial.println(!isOn);
        isOn = !isOn;
    }
    if (isOn && digitalRead(SKIPPBUTTON) == HIGH && delayBetweenHappened(skipTime, 500)) {
        Serial.println("Toggle Skipping to");
        Serial.println(!isSkipping);
        isSkipping = !isSkipping;
    }
}

/**
* Activates the outputs depending on the type passed
*
* @param type the type of the outputs(HIGH/LOW)
*/
void activateOutputs(int type) {
    digitalWrite(RELAYOUT, type);
    digitalWrite(ONLAMP, type);
}

/**
* Checks if a given delay has happened since the passed time and
* updates the variable if it did
*
* @param ref reference to the variable containing the current time
* @param duration the duration to have happened since ref time
* @return whether the delay happened
*/
boolean delayBetweenHappened(long &ref, long duration) {
    const bool happened = millis() - ref >= duration;
    if (happened) {
        ref = millis();
    }
    return happened;
}