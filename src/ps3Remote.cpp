#include "ps3Remote.h"

void notify()
{
    //--------------- Digital D-pad button events --------------

    if( Ps3.event.button_down.right ){
        last_transmission.transmission.controls &= !(LEFT_BLINKER_MASK | EMERGENCY_BLINKER_MASK);
        last_transmission.transmission.controls ^= RIGHT_BLINKER_MASK;
    }
        

    if( Ps3.event.button_down.left ){
        last_transmission.transmission.controls &= !(RIGHT_BLINKER_MASK | EMERGENCY_BLINKER_MASK);
        last_transmission.transmission.controls ^= LEFT_BLINKER_MASK;
    }

    if( Ps3.event.button_down.l1 ){
        last_transmission.transmission.controls &= LASER_MASK;
    }
        
    if( Ps3.event.button_up.l1 ){
        last_transmission.transmission.controls &= !LASER_MASK;
    }

    if( Ps3.event.button_down.r1 )
        last_transmission.transmission.controls &= AUTOMATIC_SHOT_MASK;
    if( Ps3.event.button_up.r1 )
        last_transmission.transmission.controls &= !AUTOMATIC_SHOT_MASK;

    //---------- Digital select/start/ps button events ---------
    if( Ps3.event.button_down.select ){
        last_transmission.transmission.controls &= !(RIGHT_BLINKER_MASK | LEFT_BLINKER_MASK | EMERGENCY_BLINKER_MASK);
    }

    if( Ps3.event.button_down.start ){
        last_transmission.transmission.controls &= !(RIGHT_BLINKER_MASK | LEFT_BLINKER_MASK );
        last_transmission.transmission.controls ^= EMERGENCY_BLINKER_MASK;
    }

    if( Ps3.event.button_down.ps )
        last_transmission.transmission.controls ^= LIMITER_MASK;

   if( abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2 ){
       last_transmission.transmission.x = Ps3.data.analog.stick.rx;
       last_transmission.transmission.y = Ps3.data.analog.stick.ry;
   }
}

void updateBattery(){
  switch (battery){
    case ps3_status_battery_charging:
      if ((millis() % CHARGE_BLINK_PERIOD) > CHARGE_BLINK_PERIOD / 2){
        Ps3.setPlayer(10);
      }else {
        Ps3.setPlayer(0);
      }
    break;
    
    case ps3_status_battery_full:
      Ps3.setPlayer(10);
    break;
    
    case ps3_status_battery_high:
      Ps3.setPlayer(9);
    break;
    
    case ps3_status_battery_low:
      Ps3.setPlayer(7);
    break;
    
    case ps3_status_battery_dying:
      Ps3.setPlayer(4);
    break;
    
    case ps3_status_battery_shutdown:
    if ((millis() % BAT_LOW_BLINK_PERIOD) > BAT_LOW_BLINK_PERIOD / 2){
      Ps3.setPlayer(4);
    }else {
      Ps3.setPlayer(0);
    }
    break;

  }
}

void onConnect(){
  #ifdef DEBUG
    Serial.println("Connected.");
  #endif
}

void initPS3Remote(){
    Ps3.attach(notify);
    Ps3.attachOnConnect(onConnect);
    Ps3.begin(PS3_MAC_ADDRESS);
}
