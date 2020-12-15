//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define TRIGGER_PIN 22
#define LASER_PIN 23
#define LEFT_BLINKER 21
#define RIGHT_BLINKER 19

#define SINGLE_SHOT_MASK        0b00000001   // 1
#define TRIPLE_SHOT_MASK        0b00000010   // 2
#define AUTOMATIC_SHOT_MASK     0b00000100   // 4
#define LASER_MASK              0b00001000   // 8
#define LEFT_BLINKER_MASK       0b00010000   // 16
#define EMERGENCY_BLINKER_MASK  0b00100000   // 32
#define RIGHT_BLINKER_MASK      0b01000000   // 64
#define LIMITER_MASK            0b10000000   // 128


#define LED 2

#define BLINK_PERIOD 1000

#define BURST_COUNT 3
#define SHOT_PERIOD 1000

BluetoothSerial SerialBT;

#define START_FRAME         0xABCD       // [-] Start frme definition for reliable serial communication

typedef struct {
  int8_t x;
  int8_t y;
  uint8_t controls;
} transmission_bt_t;

union Transmission_buffer{
  transmission_bt_t transmission;
  uint8_t dataReceived[sizeof(transmission_bt_t)];
};

bool lastBT_status;
unsigned long trigger_open;

// Global variables
uint8_t idx = 0;                        // Index for new data pointer
uint16_t bufStartFrame;                 // Buffer Start Frame
byte *p;                                // Pointer declaration for the new received data
byte incomingByte;
byte incomingBytePrev;

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

void hoverBoardSend(int16_t uSteer, int16_t uSpeed)
{
  // Create command
  Command.start    = (uint16_t)START_FRAME;
  Command.steer    = (int16_t)uSteer;
  Command.speed    = (int16_t)uSpeed;
  Command.checksum = (uint16_t)(Command.start ^ Command.steer ^ Command.speed);

  // Write to Serial
  Serial2.write((uint8_t *) &Command, sizeof(Command)); 
  Serial.println("sending");
}

// ########################## RECEIVE ##########################
void hoverBoardReceive()
{
  // Check for new data availability in the Serial buffer
  if (Serial2.available()) {
    incomingByte    = Serial2.read();                                 // Read the incoming byte
    bufStartFrame = ((uint16_t)(incomingByte) << 8) | incomingBytePrev;   // Construct the start frame    
  }
  else {
    return;
  }

  // If DEBUG_RX is defined print all incoming bytes
  #ifdef DEBUG_RX
    Serial.print(incomingByte);
    return;
  #endif      
  
  // Copy received data
  if (bufStartFrame == START_FRAME) {                     // Initialize if new data is detected
    p     = (byte *)&NewFeedback;
    *p++  = incomingBytePrev;
    *p++  = incomingByte;
    idx   = 2;  
  } else if (idx >= 2 && idx < sizeof(SerialFeedback)) {  // Save the new received data
    *p++  = incomingByte; 
    idx++;
  } 
  
  // Check if we reached the end of the package
  if (idx == sizeof(SerialFeedback)) {    
    uint16_t checksum;
    checksum = (uint16_t)(NewFeedback.start ^ NewFeedback.cmd1 ^ NewFeedback.cmd2 ^ NewFeedback.speedR_meas ^ NewFeedback.speedL_meas
          ^ NewFeedback.batVoltage ^ NewFeedback.boardTemp ^ NewFeedback.cmdLed);
  
    // Check validity of the new data
    if (NewFeedback.start == START_FRAME && checksum == NewFeedback.checksum) {
      // Copy the new data
      memcpy(&Feedback, &NewFeedback, sizeof(SerialFeedback));
      
      // Print data to built-in Serial
      Serial.print("1: ");   Serial.print(Feedback.cmd1);
      Serial.print(" 2: ");  Serial.print(Feedback.cmd2);
      Serial.print(" 3: ");  Serial.print(Feedback.speedR_meas);
      Serial.print(" 4: ");  Serial.print(Feedback.speedL_meas);
      Serial.print(" 5: ");  Serial.print(Feedback.batVoltage);
      Serial.print(" 6: ");  Serial.print(Feedback.boardTemp);
      Serial.print(" 7: ");  Serial.println(Feedback.cmdLed);
      
    } else {
      Serial.println("Non-valid data skipped");
    }
    idx = 0;  // Reset the index (it prevents to enter in this if condition in the next cycle)
  }
  
  // Update previous states
  incomingBytePrev  = incomingByte;
}

#define HOVER_SERIAL_BAUD   38400       // [-] Baud rate for Serial2 (used to communicate with the hoverboard)

void setup() {
  Serial.begin(115200);
  SerialBT.begin("EasyChair"); //Bluetooth device name
  Serial2.begin(HOVER_SERIAL_BAUD);
  Serial.println("The device started, now you can pair it with bluetooth!");

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(LASER_PIN, OUTPUT);
  pinMode(LEFT_BLINKER, OUTPUT);
  pinMode(RIGHT_BLINKER, OUTPUT);
  
}

uint8_t syncro1, syncro2;
Transmission_buffer last_transmission, temp_transmission;

void loop() {
  
  if (SerialBT.available()) {
    syncro2 = SerialBT.read();
    if (syncro1 == 0xAB && syncro2 == 0xCD){
      for (byte i = 0; i < sizeof(transmission_bt_t); i++){
        while (!SerialBT.available());
        last_transmission.dataReceived[i] = SerialBT.read();
      }
      if ((last_transmission.transmission.controls & (SINGLE_SHOT_MASK | AUTOMATIC_SHOT_MASK)) && trigger_open < millis()){
        trigger_open = millis() + SHOT_PERIOD;
      }

      if ((last_transmission.transmission.controls & TRIPLE_SHOT_MASK) && trigger_open < millis()){
        trigger_open = millis() + (SHOT_PERIOD * BURST_COUNT);
      }
    }
    else {
      syncro1 = syncro2;
    }
  }
  hoverBoardSend(last_transmission.transmission.x * 10, sqrt(pow(last_transmission.transmission.x, 2) + pow(last_transmission.transmission.y, 2)) * 10);
  
  digitalWrite(TRIGGER_PIN, (trigger_open > millis()));
  digitalWrite(LASER_PIN, (bool)(last_transmission.transmission.controls & LASER_MASK));
  digitalWrite(LEFT_BLINKER, !(((last_transmission.transmission.controls &  (LEFT_BLINKER_MASK | EMERGENCY_BLINKER_MASK)) > 0) && ((millis() % BLINK_PERIOD) > (BLINK_PERIOD / 2))));
  digitalWrite(RIGHT_BLINKER,!(((last_transmission.transmission.controls & (RIGHT_BLINKER_MASK | EMERGENCY_BLINKER_MASK)) > 0) && ((millis() % BLINK_PERIOD) > (BLINK_PERIOD / 2))));
  delay(50);
}
