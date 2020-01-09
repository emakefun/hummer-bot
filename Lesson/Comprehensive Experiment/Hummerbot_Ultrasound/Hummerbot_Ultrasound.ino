#include "Hummerbot.h"
#include "BluetoothHandle.h"
#include "ProtocolParser.h"
#include "KeyMap.h"
#include "debug.h"

#define IN1_PIN 6   // PWMB
#define IN2_PIN 10  // DIRB  ---  right
#define IN3_PIN 5   // DIRA  ---  left
#define IN4_PIN 9   // PWMA

#define SERVO_PIN 13
#define UL_SING_PIN 3
#define UL_RGB_PIN 2

ProtocolParser *mProtocol = new ProtocolParser();
Hummerbot hbot(mProtocol, IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN);

void setup()
{
  Serial.begin(9600);
  hbot.init();
  hbot.SetControlMode(E_ULTRASONIC_AVOIDANCE);//E_BLUETOOTH_CONTROL/E_INFRARED_TRACKING_MODE
  hbot.SetRgbUltrasonicPin(UL_SING_PIN, UL_RGB_PIN, SERVO_PIN);
  hbot.SetSpeed(0);
  hbot.mRgbUltrasonic->SetServoBaseDegree(90);
  hbot.mRgbUltrasonic->SetServoDegree(90);

}

void HandleUltrasonicAvoidance(void)
{
  uint16_t UlFrontDistance, UlLeftDistance, UlRightDistance;
  UlFrontDistance =  hbot.GetUltrasonicValue(FRONT);
  DEBUG_LOG(DEBUG_LEVEL_INFO, "UlFrontDistance =%d \n", UlFrontDistance);
   Serial.println(UlFrontDistance);
   if ((UlFrontDistance < UL_LIMIT_MIN))
  {
    hbot.SetSpeed(45);
    hbot.GoBack();
    delay(250);
  }
 else if (UlFrontDistance < UL_LIMIT_MID)
  {
    hbot.KeepStop();
    delay(100);
    UlLeftDistance = hbot.GetUltrasonicValue(LEFT);
    UlRightDistance = hbot.GetUltrasonicValue(RIGHT);
    if ((UlRightDistance > UL_LIMIT_MIN) && (UlRightDistance < UL_LIMIT_MAX) && (UlLeftDistance > UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MAX))
    {
      if (UlRightDistance > UlLeftDistance)
      {
        hbot.SetSpeed(80);
        hbot.TurnRight();
        delay(310);
      }
      else
      {
        hbot.SetSpeed(80);
        hbot.TurnLeft();
        delay(310);
      }
    }
    else if (((UlRightDistance > UL_LIMIT_MIN) && (UlRightDistance < UL_LIMIT_MAX)) || ((UlLeftDistance > UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MAX)))
    {
      if ((UlLeftDistance > UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MAX))
      {
        hbot.SetSpeed(80);
        hbot.TurnLeft();
        delay(310);
      }
      else if ((UlRightDistance > UL_LIMIT_MIN) && (UlRightDistance < UL_LIMIT_MAX))
      {
        hbot.SetSpeed(80);
        hbot.TurnRight();
        delay(310);
      }
    }
    else if ((UlRightDistance < UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MIN) )
    {
      hbot.SetSpeed(80);
      hbot.Drive(0);
      delay(510);
    }
  }
  else 
  {
    hbot.SetSpeed(45);
    hbot.GoForward();
  }
}

void loop()
{
  switch (hbot.GetControlMode()) {
    case E_ULTRASONIC_AVOIDANCE:
      DEBUG_LOG(DEBUG_LEVEL_INFO, "E_ULTRASONIC_AVOIDANCE \n");
      HandleUltrasonicAvoidance();
      break;
    default:
      break;
  }
  switch (hbot.GetStatus()) {
    case E_FORWARD:
      hbot.mRgbUltrasonic->SetRgbColor(E_RGB_ALL, RGB_WHITE);
      break;
    case E_LEFT:
      hbot.mRgbUltrasonic->SetRgbColor(E_RGB_LEFT, RGB_WHITE);
      break;
    case E_RIGHT:
      hbot.mRgbUltrasonic->SetRgbColor(E_RGB_RIGHT, RGB_WHITE);
      //   Mirage.Sing(S_OhOoh);
      break;
    case E_BACK:
      hbot.mRgbUltrasonic->SetRgbColor(E_RGB_ALL, RGB_RED);
      break;
    case E_STOP:
      hbot.mRgbUltrasonic->SetRgbColor(E_RGB_ALL, RGB_BLACK);
      break;
    case E_SPEED_UP:
      hbot.mRgbUltrasonic->SetRgbColor(E_RGB_ALL, hbot.GetSpeed() * 2.5);
      break;
    case E_SPEED_DOWN:
      hbot.mRgbUltrasonic->SetRgbColor(E_RGB_ALL, hbot.GetSpeed() * 2.5);
      break;
    default:
      break;
  }
}
