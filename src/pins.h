#ifndef PINS_H__
#define PINS_H__

#include <Arduino.h>

#define TRIGGER_PIN     22
#define LASER_PIN       23
#define LEFT_BLINKER    21
#define RIGHT_BLINKER   19
#define BT_PS3_PIN      4
#define LED             2

#define R0_LED          27
#define Y1_LED          26
#define Y2_LED          25
#define Y3_LED          33
#define G4_LED          32

void initGPIO();

#endif
