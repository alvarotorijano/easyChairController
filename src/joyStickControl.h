#ifndef JOY_STICK_CONTROL_H__
#define JOY_STICK_CONTROL_H__

#include <math.h>   //for pow()
#include <stdlib.h> //for abs()
#include "configuration.h"
#include "hoverBoardCom.h"

#define NOISE_GATE_THRESHOLD  10
#define MAX_STICK_ABS_VALUE   127
#define MAX_STICK_ABS_VALUE_CORRECTED (MAX_STICK_ABS_VALUE+1)

int smothStick(int input);
int noiseGate(int input, int threshold);

#endif
