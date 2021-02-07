#ifndef LED_INDICATOR_H__
#define LED_INDICATOR_H__

#include "hoverBoardCom.h"
#include "pins.h"
#include "configuration.h"
#include <Arduino.h>

void displayBattery(int battery);
void ledAnimation1();
void ledAnimation2();
void ledAnimation3();
void turnStripOff();

#endif