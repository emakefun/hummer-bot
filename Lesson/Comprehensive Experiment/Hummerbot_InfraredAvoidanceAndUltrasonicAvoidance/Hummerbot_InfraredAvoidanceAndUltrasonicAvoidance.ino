#include "Hummerbot.h"
#include "BluetoothHandle.h"
#include "ProtocolParser.h"
#include "KeyMap.h"
#include "debug.h"

#define IN1_PIN 6   // PWMB
#define IN2_PIN 10  // DIRB  ---  right
#define IN3_PIN 5   // DIRA  ---  left
#define IN4_PIN 9   // PWMA

#define IR_PIN 11
#define SERVO_PIN 13
#define UL_SING_PIN 3
#define UL_RGB_PIN 2
#define PS2X_CLK 11
#define PS2X_CMD 7
#define PS2X_CS  8
#define PS2X_DAT 4
#define IR_TRACING_PIN1 A0
#define IR_TRACING_PIN2 A1
#define IR_TRACING_PIN3 A2

#define PHOTOSENSITIVE_LEFT_PIN A3
#define PHOTOSENSITIVE_RIGHT_PIN A4
#define IR_AVOIDANCE_LEFT_PIN 12
#define IR_AVOIDANCE_RIGHT_PIN A5

ProtocolParser *mProtocol = new ProtocolParser();
Hummerbot hbot(mProtocol, IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN);
byte Ps2xStatus, Ps2xType;
static byte count = 0;

void setup()
{
  Serial.begin(9600);
  hbot.init();
  hbot.SetControlMode(E_ULTRASONIC_INFRARED_AVOIDANCE);
  hbot.SetRgbUltrasonicPin(UL_SING_PIN, UL_RGB_PIN, SERVO_PIN);
  hbot.SetPhotoInfraredAvoidancePin(IR_AVOIDANCE_LEFT_PIN, IR_AVOIDANCE_RIGHT_PIN, PHOTOSENSITIVE_LEFT_PIN, PHOTOSENSITIVE_RIGHT_PIN);
  hbot.SetSpeed(0);
  hbot.mRgbUltrasonic->SetServoBaseDegree(90);
  hbot.mRgbUltrasonic->SetServoDegree(90);
}

void HandleUltrasonicInfraredAvoidance(void)
{
  uint16_t RightValue, LeftValue;
  uint16_t UlFrontDistance, UlLeftDistance, UlRightDistance;
  LeftValue = hbot.GetInfraredAvoidanceValue(0);
  RightValue = hbot.GetInfraredAvoidanceValue(1);
  UlFrontDistance =  hbot.GetUltrasonicValue(FRONT);
 if (count++ > 50) {
    hbot.SendInfraredAvoidanceData();
    delay(10);
    hbot.SendUltrasonicData();
    count = 0;
  }
  if ((RightValue != IA_THRESHOLD) && (LeftValue == IA_THRESHOLD))
  {
    hbot.SetSpeed(100);
    hbot.Drive(5);
  }
  else if ((RightValue == IA_THRESHOLD) && (LeftValue != IA_THRESHOLD))
  {
    hbot.SetSpeed(100);
    hbot.Drive(175);
  }
  else
  {
    hbot.SetSpeed(40);
    hbot.GoForward();
  }
  DEBUG_LOG(DEBUG_LEVEL_INFO, "UlFrontDistance = %d \n\r", UlFrontDistance);
  if (UlFrontDistance < UL_LIMIT_MID)
  {
    hbot.KeepStop();
    if (UlFrontDistance <= UL_LIMIT_MIN || (RightValue == IA_THRESHOLD && LeftValue == IA_THRESHOLD))
    {
      hbot.SetSpeed(45);
      hbot.GoBack();
      delay(300);
      hbot.KeepStop();
    }
    UlLeftDistance = hbot.GetUltrasonicValue(LEFT);
    UlRightDistance = hbot.GetUltrasonicValue(RIGHT);
    if ((UlRightDistance > UL_LIMIT_MIN) && (UlRightDistance < UL_LIMIT_MAX) && (UlLeftDistance > UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MAX))
    {
      if (UlRightDistance >= UlLeftDistance)
      {
        hbot.SetSpeed(90);
        hbot.TurnRight();
        delay(300);
      }
      if (UlLeftDistance > UlRightDistance)
      {
        hbot.SetSpeed(90);
        hbot.TurnLeft();
        delay(300);
      }
    }
    else if (((UlRightDistance > UL_LIMIT_MIN) && (UlRightDistance < UL_LIMIT_MAX)) || ((UlLeftDistance > UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MAX)))
    {
      if ((UlLeftDistance > UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MAX))
      {
        hbot.SetSpeed(70);
        hbot.TurnLeft();
        delay(310);
      }
      else if ((UlRightDistance > UL_LIMIT_MIN) && (UlRightDistance < UL_LIMIT_MAX))
      {
        hbot.SetSpeed(70);
        hbot.TurnRight();
        delay(310);
      }
    }
   else if (UlLeftDistance <= UL_LIMIT_MIN && UlRightDistance <= UL_LIMIT_MIN )
    {
      hbot.SetSpeed(90);
      hbot.Drive(0);
      delay(700);
      hbot.KeepStop();
    }
    hbot.KeepStop();
  }
}

void loop()
{
  switch (hbot.GetControlMode()) {
    case E_ULTRASONIC_INFRARED_AVOIDANCE:
      DEBUG_LOG(DEBUG_LEVEL_INFO, "E_ULTRASONIC_INFRARED_AVOIDANCE \n");
      HandleUltrasonicInfraredAvoidance();
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
