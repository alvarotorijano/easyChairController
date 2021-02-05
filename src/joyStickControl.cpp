#include "joyStickControl.h"

uint8_t idx = 0;

int smothStick(int input){
  #ifdef CUADRATIC_SMOTHERING
  return ((pow(input, 2) + input) / (float)MAX_STICK_ABS_VALUE_CORRECTED) * ((float)MAX_STEERING_VALUE / MAX_STICK_ABS_VALUE_CORRECTED);
  #endif

  #ifdef CUBIC_SMOTHERING
  return abs((pow(input, 3) + input) / (float)pow(MAX_STICK_ABS_VALUE, 2)) * ((float)MAX_STEERING_VALUE / MAX_STICK_ABS_VALUE);
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
