#ifndef BT_COMS_H__
#define BT_COMS_H__

#include "transmission.h"
#include "BluetoothSerial.h"
#include "configuration.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

extern BluetoothSerial SerialBT;

Transmission_buffer BTReceive();
void initBT();

#endif
