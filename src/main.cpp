#include <Arduino.h>
#include "pins.h"
#include "ps3Remote.h"
#include "joyStickControl.h"
#include "configuration.h"
#include "hoverBoardCom.h"
#include "triggerManager.h"
#include "BTComs.h"
#include "blinkerManager.h"

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
  
  Serial2.begin(HOVER_SERIAL_BAUD);
}

void loop() {
  static int steering;
  static int throttle;
  static Transmission_buffer temp_transmission;
  if (initialBTSwitch != digitalRead(BT_PS3_PIN)){
    delay(1000);
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
      return;
    }
    updateBattery();
  }

  triggerGun(last_transmission.transmission.controls);
  triggerBlinker(last_transmission.transmission.controls);

      #ifdef NOISE_GATE
      steering = smothStick(noiseGate(last_transmission.transmission.x, NOISE_GATE_THRESHOLD));// last_transmission.transmission.x * (float)2000 / 128;
      throttle = smothStick(noiseGate(last_transmission.transmission.y, NOISE_GATE_THRESHOLD));
    #else
      steering = smothStick(last_transmission.transmission.x);// last_transmission.transmission.x * (float)2000 / 128;
      throttle = smothStick(last_transmission.transmission.y);
    #endif
    
    if(last_transmission.transmission.controls & LIMITER_MASK && throttle > MAX_SPEED){
      throttle = MAX_SPEED;
    }
    if (last_transmission.transmission.y < 0) {
      throttle = throttle * -1;
    }
    
    hoverBoardSend(steering, throttle);
    delay(50);

}
