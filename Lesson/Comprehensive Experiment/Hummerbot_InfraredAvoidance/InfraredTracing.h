
#ifndef _INFRAREDTRACING_H_
#define _INFRAREDTRACING_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>


#define  IT_ALL_BLACK     0b000
#define  IT_ALL_WHITE     0b111
#define  IT_RIGHT1         0b011
#define  IT_RIGHT2         0b001
#define  IT_CENTER        0b101
#define  IT_LEFT1        0b110
#define  IT_LEFT2        0b100


// information for the interrupt handler
typedef enum {
  E_INFRARED_SENSOR_2,
  E_INFRARED_SENSOR_3,
  E_INFRARED_SENSOR_4,
  E_INFRARED_SENSOR_5
} E_INFRARWD_TRACING_MODE;


class InfraredTracing
{
  private:
    byte InfraredTracingPin1, InfraredTracingPin2, InfraredTracingPin3;
    byte value;
    double irDelayTime;
    byte dat = 0;
    byte ItPins[3];

  public:
    InfraredTracing(byte pin1, byte pin2, byte pin3);
    void begin(void);
    E_INFRARWD_TRACING_MODE InfraredTracingMode;
    byte GetValue(void);
    byte GetLeftValue(void);
    byte GetMiddleValue(void);
    byte GetRightValue(void);
};
#endif
