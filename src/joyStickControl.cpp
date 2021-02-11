#include "joyStickControl.h"

uint8_t idx = 0;
int speed_limit;

int smothStick(int input, int maxValue){
  #ifdef CUADRATIC_SMOTHERING
  return ((pow(input, 2) + input) / (float)MAX_STICK_ABS_VALUE_CORRECTED) * ((float)maxValue / MAX_STICK_ABS_VALUE_CORRECTED);
  #endif

  #ifdef CUBIC_SMOTHERING
  return abs((pow(input, 3) + input) / (float)pow(MAX_STICK_ABS_VALUE, 2)) * ((float)maxValue / MAX_STICK_ABS_VALUE);
  #endif
}

int noiseGate(int input, int threshold){
  if (abs(input) < threshold){
    return 0;
  }
  else {
    if (input < 0){
      return round((input + threshold)*(((float)MAX_STICK_ABS_VALUE_CORRECTED + threshold + 1) / (float)MAX_STICK_ABS_VALUE));
    } else {
      return round((input - threshold)*(((float)MAX_STICK_ABS_VALUE_CORRECTED + threshold + 1) / (float)MAX_STICK_ABS_VALUE));
    }
  }
}

void setMaxSpeed(int speed){
  speed_limit = speed;
}
