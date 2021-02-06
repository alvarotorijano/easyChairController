#ifndef PS3_REMOTE_H__
#define PS3_REMOTE_H__

#include <Ps3Controller.h>
#include "configuration.h"
#include "transmission.h"

#define CHARGE_BLINK_PERIOD   1000
#define BAT_LOW_BLINK_PERIOD  500

void notify();
void updateBattery();
void onConnect();
void initPS3Remote();

extern int battery;

#endif
