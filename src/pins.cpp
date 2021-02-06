#include "pins.h"

void initGPIO(){
    pinMode(BT_PS3_PIN, INPUT);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(LASER_PIN, OUTPUT);
    pinMode(LEFT_BLINKER, OUTPUT);
    pinMode(RIGHT_BLINKER, OUTPUT);
    pinMode(LED, OUTPUT);

    digitalWrite(TRIGGER_PIN, LOW);
    digitalWrite(LASER_PIN, LOW);
    digitalWrite(LEFT_BLINKER, LOW);
    digitalWrite(RIGHT_BLINKER, LOW);
    digitalWrite(LED, LOW);
}