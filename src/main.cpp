#include <Arduino.h>
#include "pins.h"
#include "ps3Remote.h"
#include "joyStickControl.h"
#include "configuration.h"
#include "hoverBoardCom.h"
#include "triggerManager.h"
#include "BTComs.h"
#include "blinkerManager.h"
#include "ledIndicator.h"

bool initialBTSwitch;



void setup() {

  initGPIO();

  if (digitalRead(BT_PS3_PIN) == BT){
    initBT();
    initialBTSwitch = BT;
  }
  else {
    initPS3Remote();
    initialBTSwitch = PS3;
    digitalWrite(LED, HIGH);
  }

  #ifdef DEBUG
  Serial.begin(USB_BAUDRATE);
  if (initialBTSwitch == BT){
    Serial.println("Bluetooth mode");
  }
  else {
    Serial.println("PS3 controller mode");
  }
  #endif
  
  initHover();
  
  ledAnimation2();
  ledAnimation3();
  ledAnimation2();
  ledAnimation3();
  ledAnimation1();

  delay(SETUP_DELAY);

}

void loop() {
  static int steering;
  static int throttle;
  static Transmission_buffer temp_transmission;


  if (initialBTSwitch != digitalRead(BT_PS3_PIN)){
    delay(RESET_DELAY);
    ESP.restart();
  }
  
  if ( initialBTSwitch == BT) {  
    temp_transmission = BTReceive();
    if (memcmp(&last_transmission, &temp_transmission, sizeof(Transmission_buffer))){
      last_transmission = temp_transmission;
      triggerGun(last_transmission.transmission.controls);
    }
  }
  else {
    if(!Ps3.isConnected()){
      digitalWrite(LED, LOW);
      return;
    }
    digitalWrite(LED, HIGH);
    updateBattery();
  }
  
  triggerGun(last_transmission.transmission.controls);
  triggerBlinker(last_transmission.transmission.controls);

  if(last_transmission.transmission.controls & LIMITER_MASK){
    setMaxSpeed(LIMITED_SPEED);
  }
  else {
    setMaxSpeed(MAX_SPEED);
  }

  #ifdef NOISE_GATE
    steering = smothStick(noiseGate(last_transmission.transmission.x, NOISE_GATE_THRESHOLD), MAX_STEERING_VALUE);// last_transmission.transmission.x * (float)2000 / 128;
    throttle = smothStick(noiseGate(last_transmission.transmission.y, NOISE_GATE_THRESHOLD), speed_limit);
  #else
    steering = smothStick(last_transmission.transmission.x, MAX_STEERING_VALUE);// last_transmission.transmission.x * (float)2000 / 128;
    throttle = smothStick(last_transmission.transmission.y, speed_limit);
  #endif

  if (last_transmission.transmission.y < 0) {
    throttle = throttle * -1;
  }
  if (last_transmission.transmission.x < 0) {
    steering = steering * -1;
  }

  #ifndef DEBUG
    hoverBoardReceive();
    hoverBoardSend(steering, throttle);
  #else
    Serial.println("steering: " + String (steering));
    Serial.println("throttle: " + String (throttle));
  #endif
  
  displayBattery(Feedback.batVoltage);
  delay(LOOP_DELAY);

}
