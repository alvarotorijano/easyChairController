#include "blinkerManager.h"

void triggerBlinker(byte control){
    digitalWrite(LASER_PIN, (bool)(control & LASER_MASK));
    digitalWrite(LEFT_BLINKER, !(((control &  (LEFT_BLINKER_MASK | EMERGENCY_BLINKER_MASK)) > 0) && ((millis() % BLINK_PERIOD) > (BLINK_PERIOD / 2))));
    digitalWrite(RIGHT_BLINKER,!(((control & (RIGHT_BLINKER_MASK | EMERGENCY_BLINKER_MASK)) > 0) && ((millis() % BLINK_PERIOD) > (BLINK_PERIOD / 2))));
}
