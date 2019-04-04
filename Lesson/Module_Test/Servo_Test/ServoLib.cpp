#include "ServoLib.h"

Servo_Test::Servo_Test(byte servo_pin)
{
    ServoPin = servo_pin;
    pinMode(ServoPin,OUTPUT);
}
void Servo_Test::SetServoDegree(int Angle)
{
  int servo_degree = Angle;
  for (int i = 0; i < 80; i++) {
    float pulsewidth = (servo_degree * 11) + 500;
    digitalWrite(ServoPin, HIGH);   //Set the servo interface level to high
    delayMicroseconds(pulsewidth);  //The number of microseconds of the delay pulse width value
    digitalWrite(ServoPin, LOW);    //Set the servo interface level to low
    delayMicroseconds(20000 - pulsewidth);
  }
  delay(250);
}
