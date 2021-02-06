#ifndef PINS_H__
#define PINS_H__

#include <Arduino.h>

#define TRIGGER_PIN 22
#define LASER_PIN 23
#define LEFT_BLINKER 21
#define RIGHT_BLINKER 19
#define BT_PS3_PIN 4
#define LED 2

#define PS3 1
#define BT  0

void initGPIO();

#endif
