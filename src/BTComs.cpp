#include "BTComs.h"

Transmission_buffer BTReceive(){
    static Transmission_buffer transmission = {};
    static uint8_t syncro1, syncro2;
    if (SerialBT.available()) {
      syncro2 = SerialBT.read();

      if (syncro1 == 0xAB && syncro2 == 0xCD){
        for (byte i = 0; i < sizeof(transmission_bt_t); i++){
          while (!SerialBT.available());
          transmission.dataReceived[i] = SerialBT.read();
        }
      }
      else {
        syncro1 = syncro2;
      }
    }
    return transmission;
}

void initBT(){
    SerialBT.begin(DEVICE_NAME); //Bluetooth device name
}
