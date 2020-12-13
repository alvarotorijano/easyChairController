//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define TRIGGER_PIN 2
#define LASER_PIN 3
#define BT_STATUS_PIN 4
#define BT_ENABLE_PIN 5
#define LEFT_BLINKER 6
#define RIGHT_BLINKER 7

#define SINGLE_SHOT_MASK        0b00000001   // 1
#define TRIPLE_SHOT_MASK        0b00000010   // 2
#define AUTOMATIC_SHOT_MASK     0b00000100   // 4
#define LASER_MASK              0b00001000   // 8
#define LEFT_BLINKER_MASK       0b00010000   // 16
#define EMERGENCY_BLINKER_MASK  0b00100000   // 32
#define RIGHT_BLINKER_MASK      0b01000000   // 64
#define LIMITER_MASK            0b10000000   // 128


#define LED 26

#define BLINK_PERIOD 2000

#define BURST_COUNT 3
#define SHOT_PERIOD 1000

BluetoothSerial SerialBT;

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

void setup() {
  Serial.begin(115200);
  SerialBT.begin("EasyChair"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
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
      Serial.println(
      "BlinkL: " +          String((last_transmission.transmission.controls & LEFT_BLINKER_MASK) > 0) + " " +
      "BlinkR: " +          String((last_transmission.transmission.controls & RIGHT_BLINKER_MASK) > 0) + " " +
      "Danger: " +          String((last_transmission.transmission.controls & EMERGENCY_BLINKER_MASK) > 0) + " " +
      "Laser: " +           String((last_transmission.transmission.controls & LASER_MASK) > 0) + " " +
      "single_shot: " +     String((last_transmission.transmission.controls & SINGLE_SHOT_MASK) > 0) + " " +
      "triple_shot: " +     String((last_transmission.transmission.controls & TRIPLE_SHOT_MASK) > 0) + " " +
      "automatic_shot: " +  String((last_transmission.transmission.controls & AUTOMATIC_SHOT_MASK) > 0) + " " +
      "limiter: " +         String((last_transmission.transmission.controls & LIMITER_MASK) > 0) + " " +
      "x: " + (int)last_transmission.transmission.x + " " +
      "y: " + (int)last_transmission.transmission.y + " " +
      "status: " + String(last_transmission.transmission.controls)
      );
    }
    else {
      syncro1 = syncro2;
    }
  }
  
  digitalWrite(TRIGGER_PIN, (trigger_open - millis()) > 0);
  digitalWrite(LASER_PIN, last_transmission.transmission.controls & LASER_MASK);
  digitalWrite(LEFT_BLINKER, !(last_transmission.transmission.controls & ( LEFT_BLINKER_MASK | EMERGENCY_BLINKER_MASK) && (millis() % BLINK_PERIOD > BLINK_PERIOD / 2)));
  digitalWrite(RIGHT_BLINKER,!(last_transmission.transmission.controls & (RIGHT_BLINKER_MASK | EMERGENCY_BLINKER_MASK) && (millis() % BLINK_PERIOD > BLINK_PERIOD / 2)));

  delay(20);
}
