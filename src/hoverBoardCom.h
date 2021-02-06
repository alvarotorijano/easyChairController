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
extern SerialCommand Command;

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
extern SerialFeedback Feedback;
extern SerialFeedback NewFeedback;

void hoverBoardReceive();
void hoverBoardSend(int16_t uSteer, int16_t uSpeed);

// NO DEFINAS VARIABLES EN UN FICHERO .H, DECLARALAS SOLO

// Aqui solo defines, en el hoverBoardCom.cpp ya le das valor a la variable   

// Global variables
extern uint8_t idx;                        // Index for new data pointer  // ESTO PUEDE SER PROBLEMATICO
extern uint16_t bufStartFrame;                 // Buffer Start Frame
extern byte *p;                                // Pointer declaration for the new received data
extern byte incomingByte;
extern byte incomingBytePrev;

#endif
