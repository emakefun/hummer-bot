
#ifndef _INFRAREDAVOIDABCE_H_
#define _INFRAREDAVOIDABCE_H_

/* Includes ------------------------------------------------------------------*/
#include <Arduino.h>

#define  IOA_ALL_YES       0b00 // IOA represent Infrared obstacle avoidance
#define  IOA_ALL_NO        0b11 
#define  IOA_LEFT          0b01 
#define  IOA_RIGHT         0b10 
#define IA_THRESHOLD 0
class InfraredAvoidance
{
  private:
    byte LeftInfraredAvoidancePin,RightInfraredAvoidancePin;
    byte LeftPhotoresistorPin,RightPhotoresistorPin;
    byte dat = 0;
    byte ItPins[2];

  public:
    InfraredAvoidance(byte pin1, byte pin2, byte pin3, byte pin4);
	InfraredAvoidance(byte pin1, byte pin2);
	void  PhotosensitivePin(byte pin1, byte pin2);
    uint8_t GetInfraredAvoidanceValue(void);
    uint8_t GetLeftInfraredAvoidanceValue(void);
    uint8_t GetRightInfraredAvoidanceValue(void);
    int GetLeftPhotoresistorValue(void);
    int GetRightPhotoresistorValue(void);
};
#endif
