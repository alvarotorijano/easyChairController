#include "pins.h"

void initGPIO(){

    pinMode(BT_PS3_PIN, INPUT);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(LASER_PIN, OUTPUT);
    pinMode(LEFT_BLINKER, OUTPUT);
    pinMode(RIGHT_BLINKER, OUTPUT);
    pinMode(LED, OUTPUT);

    pinMode(R0_LED, OUTPUT);
    pinMode(Y1_LED, OUTPUT);
    pinMode(Y2_LED, OUTPUT);
    pinMode(Y3_LED, OUTPUT);
    pinMode(G4_LED, OUTPUT);

    digitalWrite(TRIGGER_PIN, LOW);
    digitalWrite(LASER_PIN, LOW);
    digitalWrite(LEFT_BLINKER, LOW);
    digitalWrite(RIGHT_BLINKER, LOW);
    digitalWrite(LED, LOW);

    digitalWrite(R0_LED, LOW);
    digitalWrite(Y1_LED, LOW);
    digitalWrite(Y2_LED, LOW);
    digitalWrite(Y3_LED, LOW);
    digitalWrite(G4_LED, LOW);
}