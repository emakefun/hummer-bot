#ifndef _RGBULTRASONIC_H_
#define _RGBULTRASONIC_H_

/* Includes ------------------------------------------------------------------*/
#include <Arduino.h>
#include "RGBLed.h"

#define UL_LIMIT_MIN 5
#define UL_LIMIT_MID 10
#define UL_LIMIT_MAX 400

class RgbUltrasonic
{
  private:
    byte SingPin, RgbPin, ServoPin;
    uint8_t ServoBaseDegree;
    

  public:
    RGBLed *mRgb;
    byte determine;
    uint16_t FrontDistance;
    uint16_t RightDistance;
    uint16_t LeftDistance;
    RgbUltrasonic(byte trig_pin, byte echo_pin, byte servo_pin);
    RgbUltrasonic(byte sing_pin, byte rgb_pin);
    void  ServoPIN(byte servo_pin);   
    uint16_t GetUltrasonicFrontDistance();
    uint16_t GetUltrasonicLeftDistance();
    uint16_t GetUltrasonicRightDistance();
    void SetServoBaseDegree(uint8_t base);
    void SetServoDegree(int degree);
};
#endif
