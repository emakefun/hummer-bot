#include "RgbUltrasonic.h"

RgbUltrasonic::RgbUltrasonic(byte sing_pin, byte rgb_pin, byte servo_pin)
{
    SingPin = sing_pin;
    RgbPin = rgb_pin;
    ServoPin = servo_pin;
    pinMode(ServoPin,OUTPUT);
    mRgb = new RGBLed(RgbPin,6);
}
RgbUltrasonic::RgbUltrasonic(byte sing_pin, byte rgb_pin)
{
    SingPin = sing_pin;
    RgbPin = rgb_pin;
    mRgb = new RGBLed(RgbPin,6);
}

void RgbUltrasonic::ServoPIN(byte servo_pin)
{
  ServoPin = servo_pin;
  pinMode(ServoPin,OUTPUT);
}
uint16_t RgbUltrasonic::GetUltrasonicFrontDistance()
{
    unsigned long Time_Echo_us = 0;
    pinMode(SingPin, OUTPUT);
    digitalWrite(SingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(SingPin, HIGH);
    delayMicroseconds(20);
    digitalWrite(SingPin, LOW);
    pinMode(SingPin, INPUT);
    Time_Echo_us = pulseIn(SingPin, HIGH);
    if ((Time_Echo_us < 60000) && (Time_Echo_us > 1)) {
      FrontDistance = Time_Echo_us / 58.00;
      return FrontDistance;
    }
    return 0;
}

uint16_t RgbUltrasonic::GetUltrasonicLeftDistance()
{
    SetServoDegree(180);
    LeftDistance = GetUltrasonicFrontDistance();
    SetServoDegree(90);
    return LeftDistance;
}

uint16_t RgbUltrasonic::GetUltrasonicRightDistance()
{
    SetServoDegree(20);
    RightDistance = GetUltrasonicFrontDistance();
    SetServoDegree(90);
    return RightDistance;
}

void RgbUltrasonic::SetServoBaseDegree(uint8_t base)
{
    ServoBaseDegree = base;
}

void RgbUltrasonic::SetServoDegree(int Angle)
{
	int Degree = Angle;
	int servo_degree;
	if (Degree > 360) {
		return;
	}
	if (Degree == 90 || Degree == 270) {
		servo_degree = ServoBaseDegree;
	} else if (Degree >= 0 && Degree <= 180) {
		servo_degree = ServoBaseDegree - 90 + Degree;   // 180-degree-diff
	}
	for (int i = 0; i < 80; i++) {
		float pulsewidth = (servo_degree * 11) + 350;
		digitalWrite(ServoPin, HIGH);   //Set the servo interface level to high
		delayMicroseconds(pulsewidth);  //The number of microseconds of the delay pulse width value
		digitalWrite(ServoPin, LOW);    //Set the servo interface level to low
		delayMicroseconds(20000 - pulsewidth);
	}
	delay(250);
}
