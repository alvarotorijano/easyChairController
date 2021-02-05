#ifndef HOVER_BOARD_COM_H__
#define HOVER_BOARD_COM_H__

#include <Arduino.h>

#define MAX_STEERING_VALUE    2000
#define START_FRAME           0xABCD       // [-] Start frme definition for reliable serial communication

#define HOVER_SERIAL_BAUD   38400       // [-] Baud rate for Serial2 (used to communicate with the hoverboard)

typedef struct{
   uint16_t start;
   int16_t  steer;
   int16_t  speed;
   uint16_t checksum;
} SerialCommand;
SerialCommand Command;

typedef struct{
   uint16_t start;
   int16_t  cmd1;
   int16_t  cmd2;
   int16_t  speedR_meas;
   int16_t  speedL_meas;
   int16_t  batVoltage;
   int16_t  boardTemp;
   uint16_t cmdLed;
   uint16_t checksum;
} SerialFeedback;
SerialFeedback Feedback;
SerialFeedback NewFeedback;

void hoverBoardReceive();
void hoverBoardSend(int16_t uSteer, int16_t uSpeed);

// Global variables
uint8_t idx = 0;                        // Index for new data pointer
uint16_t bufStartFrame;                 // Buffer Start Frame
byte *p;                                // Pointer declaration for the new received data
byte incomingByte;
byte incomingBytePrev;

#endif
