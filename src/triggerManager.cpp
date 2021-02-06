#include "triggerManager.h"

void triggerGun(byte control){
    static unsigned long trigger_open = 0;
    if ((control & (SINGLE_SHOT_MASK | AUTOMATIC_SHOT_MASK)) && trigger_open < millis() + TRIGGER_DELAY){
          trigger_open = millis() + SHOT_PERIOD;
    }

    if ((control & TRIPLE_SHOT_MASK) && trigger_open < millis() + TRIGGER_DELAY){
        trigger_open = millis() + (SHOT_PERIOD * BURST_COUNT);
    }
    digitalWrite(TRIGGER_PIN, (trigger_open > millis()));
}
