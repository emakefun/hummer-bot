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

void RgbUltrasonic::SetRgbColor(E_RGB_INDEX index, long Color)
{
    if (index == E_RGB_ALL) {
        mRgb->setColor(0, Color);
    } else if (index == E_RGB_RIGHT) {
        mRgb->setColor(1, Color);
        mRgb->setColor(2, Color);
        mRgb->setColor(3, Color);
    } else if (index == E_RGB_LEFT) {
        mRgb->setColor(4, Color);
        mRgb->setColor(5, Color);
        mRgb->setColor(6, Color);
    }
    mRgb->show();
}

void RgbUltrasonic::SetRgbIndexColor(int start_index, int end_index, long Color)
{
    for (byte i = start_index; i <= end_index; i++) {
        mRgb->setColor(i, Color);
    }
    mRgb->show();
}

void RgbUltrasonic::SetRgbEffect(E_RGB_INDEX index, long Color, uint8_t effect = E_EFFECT_NONE)
{
	  byte start = 1, end = 6;
    if (index == E_RGB_RIGHT) {
        start = 4;
        end = 6;
    } else if (index == E_RGB_LEFT) {
        start = 1;
        end = 3;
    }

    switch((E_RGB_EFFECT)effect) {
		case E_EFFECT_NONE:
        //Serial.println("E_EFFECT_NONE");
		    SetRgbIndexColor(start, end, Color);
			  break;
    case E_EFFECT_BREATHING:
        //Serial.println("E_EFFECT_BREATHING");
        for (int i = 0; i < 256; i++) {
            mRgb->setBrightness(i);
            SetRgbIndexColor(start, end, Color);
            //delay((255 - i)/2);
            delay((i < 20)? 60 :(255/i));
        }
        for (int i = 255; i >= 0; i--) {
            mRgb->setBrightness(i);
            SetRgbIndexColor(start, end, Color);
            delay((i < 20)? 60 :(255/i));
        }
        mRgb->setBrightness(255);
        break;
    case E_EFFECT_ROTATE:
        //Serial.println("E_EFFECT_ROTATE");
		for (byte i = 0; i < 4; i++) {
			mRgb->setColor(start, Color);
			mRgb->setColor(start+1, 0);
			mRgb->setColor(start+2, 0);
			if (index == E_RGB_ALL) {
				mRgb->setColor(end-2, Color);
				mRgb->setColor(end-1, 0);
				mRgb->setColor(end, 0);
			}
			mRgb->show();
			delay(150);
			mRgb->setColor(start, 0);
			mRgb->setColor(start+1, Color);
			mRgb->setColor(start+2, 0);
			if (index == E_RGB_ALL) {
				mRgb->setColor(end-2, 0);
				mRgb->setColor(end-1, Color);
				mRgb->setColor(end, 0);
			}
			mRgb->show();
			delay(150);
			mRgb->setColor(start, 0);
			mRgb->setColor(start+1, 0);
			mRgb->setColor(start+2, Color);
			if (index == E_RGB_ALL) {
				mRgb->setColor(end-2, 0);
				mRgb->setColor(end-1, 0);
				mRgb->setColor(end, Color);
			}
			mRgb->show();
			delay(150);
		}
		SetRgbIndexColor(1, 6, 0);
		break;
      case E_EFFECT_FLASH:
          //Serial.println("E_EFFECT_FLASH");
          for (byte i = 0; i < 6; i++) {
              SetRgbIndexColor(start, end, Color);
              delay(150);
              SetRgbIndexColor(start, end, 0);
              delay(150);
          }
          break;
      }
}

