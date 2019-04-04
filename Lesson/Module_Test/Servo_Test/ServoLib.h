#ifndef _SERVO_TEST_H_
#define _SERVO_TEST_H_

#include <Arduino.h>

class Servo_Test
{
  private:
    byte  ServoPin;
    uint8_t ServoBaseDegree;
    
  public:
    byte determine;
    Servo_Test(byte servo_pin);
    void SetServoDegree(int degree);
};
#endif
