#ifndef TRANSMISSION_H__
#define TRANSMISSION_H__

#include <Arduino.h>

typedef struct {
  int8_t x;
  int8_t y;
  uint8_t controls;
} transmission_bt_t;

union Transmission_buffer{
  transmission_bt_t transmission;
  uint8_t dataReceived[sizeof(transmission_bt_t)];
};

// This must be global because of the PS3 callback
Transmission_buffer last_transmission;

#endif
