#ifndef CONFIGURATION_H__
#define CONFIGURATION_H__

#define DEBUG_LOOP_DELAY      500
#define NORMAL_LOOP_DELAY     50

//#define DEBUG

#ifdef DEBUG
#define SETUP_DELAY           2000
#define LOOP_DELAY            DEBUG_LOOP_DELAY
#else
#define SETUP_DELAY           0
#define LOOP_DELAY            NORMAL_LOOP_DELAY
#endif

#define RESET_DELAUY          1000

#define USB_BAUDRATE          9600

#define BLINK_PERIOD          1000

#define BURST_COUNT           3
#define SHOT_PERIOD           110
#define TRIGGER_DELAY         500

#define MAX_SPEED             500

#define DEVICE_NAME           "EasyChair"

#define PS3_MAC_ADDRESS       "00:19:C1:C0:C7:86"

///Control options
#define NOISE_GATE
// define CUADRATIC_SMOTHERING
#define CUBIC_SMOTHERING

#ifdef CUADRATIC_SMOTHERING
  #ifdef CUBIC_SMOTHERING
    #error TOO_MANY_SMOTHERING_FUNCTIONS_DEFINED
  #endif
#endif

#ifndef CUADRATIC_SMOTHERING
    #ifndef CUBIC_SMOTHERING
        #define CUADRATIC_SMOTHERING
    #endif
#endif


#define SINGLE_SHOT_MASK        0b00000001   // 1
#define TRIPLE_SHOT_MASK        0b00000010   // 2
#define AUTOMATIC_SHOT_MASK     0b00000100   // 4
#define LASER_MASK              0b00001000   // 8
#define LEFT_BLINKER_MASK       0b00010000   // 16
#define EMERGENCY_BLINKER_MASK  0b00100000   // 32
#define RIGHT_BLINKER_MASK      0b01000000   // 64
#define LIMITER_MASK            0b10000000   // 128

#endif
