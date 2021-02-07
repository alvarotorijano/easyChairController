
#include "ledIndicator.h"

const byte ledArray [] = {R0_LED, Y1_LED, Y2_LED, Y3_LED, G4_LED};
void displayBattery(int battery){
    uint8_t nLeds = 0;
    nLeds = (battery - MIN_BATTERY_VALUE) / ((MAX_BATTERY_VALUE - MIN_BATTERY_VALUE) / (float)sizeof(ledArray));
    nLeds += 1;

    turnStripOff();
    
    for (uint8_t i = 0; (i < sizeof(ledArray)) && (i < nLeds); i++){
        digitalWrite(ledArray[i], HIGH);
    } 
}

void ledAnimation1(){
    turnStripOff();
    for(uint8_t i = 0; i < sizeof(ledArray); i++){
        delay(100);
        digitalWrite(ledArray[i], HIGH);
        delay(100);
    }
    turnStripOff();
}

void ledAnimation2(){
    turnStripOff();
    for(uint8_t i = 0; i < sizeof(ledArray); i++){
        delay(50);
        turnStripOff();
        digitalWrite(ledArray[i], HIGH);
        delay(50);
    }
    turnStripOff();
}

void ledAnimation3(){
    turnStripOff();
    for(uint8_t i = 0; i < sizeof(ledArray); i++){
        delay(50);
        turnStripOff();
        digitalWrite(ledArray[sizeof(ledArray) - 1 -i], HIGH);
        delay(50);
    }
    turnStripOff();
}


void turnStripOff(){
    for(uint8_t i = 0; i < sizeof(ledArray); i++){
        digitalWrite(ledArray[i], LOW);
    }
}
