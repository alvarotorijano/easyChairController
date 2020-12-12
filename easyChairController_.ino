#include <SoftwareSerial.h>
#include <stdint.h>

#define BT_BAUDRATE 9600

#define TRIGGER_PIN 2
#define LASER_PIN 3
#define BT_STATUS_PIN 4
#define BT_ENABLE_PIN 5
#define LEFT_BLINKER 6
#define RIGHT_BLINKER 7

#define LED LED_BUILTIN

#define BLINK_PERIOD 2000

#define BURST_COUNT 3
#define SHOT_PERIOD 1000

typedef struct {
  uint16_t initialization; //0xABCD
  uint16_t x;
  uint16_t y;
  uint8_t single_shot;
  uint8_t triple_shot;
  uint8_t automatic_shot;
  uint8_t laser;
  uint8_t left_blinker;
  uint8_t emergency_blinker;
  uint8_t right_blinker;
} transmission_bt_t;

bool lastBT_status;
transmission_bt_t last_transmission;
unsigned long trigger_open;

void setup() {
  Serial1.begin(BT_BAUDRATE, SERIAL_8O1);
  //Serial1.begin(BT_BAUDRATE);
  Serial.begin(9600);
  Serial.println("EasyChair started");
  pinMode(BT_STATUS_PIN, INPUT);

  pinMode(BT_ENABLE_PIN, OUTPUT);
  digitalWrite(BT_ENABLE_PIN, HIGH);
  
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  memset (&last_transmission, 0, sizeof(transmission_bt_t));
  trigger_open = millis();
  lastBT_status = false;
  
}


void loop() {

//*/

  if ( lastBT_status == false && digitalRead(BT_STATUS_PIN) == true){
    delay(1000);
  }

  if ( lastBT_status == true && digitalRead(BT_STATUS_PIN) == false){
    digitalWrite(BT_ENABLE_PIN, LOW);
    delay(500);
    digitalWrite(BT_ENABLE_PIN, HIGH);
  }
  
  if ( lastBT_status != digitalRead(BT_STATUS_PIN)){
    lastBT_status = digitalRead(BT_STATUS_PIN);
  }
  //*/

  if (Serial1.available() == sizeof(transmission_bt_t)){
    Serial.println("mensaje recibido");
    transmission_bt_t temp;
    for (byte i = 0; i < sizeof(transmission_bt_t); i++){
      ((byte *)(&temp))[i] = Serial1.read();
    }
    if (temp.initialization == 0xABCD){
      last_transmission = temp;
      if (last_transmission.single_shot && trigger_open < millis()){
        trigger_open = millis() + SHOT_PERIOD;
      }

      if (last_transmission.single_shot && trigger_open < millis()){
        trigger_open = millis() + (SHOT_PERIOD * BURST_COUNT);
      }
      
      if (last_transmission.automatic_shot && trigger_open < millis()){
        trigger_open = millis() + SHOT_PERIOD ;
      }
    }
    char mensaje[50];
    sprintf(mensaje, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", last_transmission.initialization, last_transmission.x, last_transmission.y, last_transmission.single_shot, last_transmission.triple_shot, last_transmission.automatic_shot, last_transmission.laser, last_transmission.left_blinker, last_transmission.emergency_blinker, last_transmission.right_blinker);
    //Serial.println(last_transmission.right_blinker);
    Serial.println(mensaje);
  }
  
  digitalWrite(TRIGGER_PIN, (trigger_open - millis()) > 0);
  digitalWrite(LASER_PIN, last_transmission.laser);
  digitalWrite(LEFT_BLINKER, !((last_transmission.left_blinker  |  last_transmission.emergency_blinker) && (millis() % BLINK_PERIOD > BLINK_PERIOD / 2)));
  digitalWrite(RIGHT_BLINKER,!((last_transmission.right_blinker |  last_transmission.emergency_blinker) && (millis() % BLINK_PERIOD > BLINK_PERIOD / 2)));

  //Serial.println(last_transmission.left_blinker);

}
