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

#define PHOTOSENSITIVE_LEFT_PIN A3
#define PHOTOSENSITIVE_RIGHT_PIN A4
#define IR_AVOIDANCE_LEFT_PIN 12
#define IR_AVOIDANCE_RIGHT_PIN A5

ProtocolParser *mProtocol = new ProtocolParser();
Hummerbot hbot(mProtocol, IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN);
static byte count = 0;

void setup()
{
  Serial.begin(9600);
  hbot.init();
  hbot.SetControlMode(E_LIGHT_SEEKING_MODE);
  hbot.SetRgbUltrasonicPin(UL_SING_PIN, UL_RGB_PIN, SERVO_PIN);
  hbot.SetPhotoInfraredAvoidancePin(IR_AVOIDANCE_LEFT_PIN, IR_AVOIDANCE_RIGHT_PIN, PHOTOSENSITIVE_LEFT_PIN, PHOTOSENSITIVE_RIGHT_PIN);
  hbot.SetSpeed(0);
}

void HandleLightSeeking()
{
  float LeftValue, RightValue;
  int Angle;
  hbot.SetSpeed(80);
  LeftValue = hbot.GetPhotosensitive(0)/10;
  RightValue = hbot.GetPhotosensitive(1)/10;
  if ((LeftValue > 30) && (RightValue > 30))
  {
    hbot.KeepStop();
  } else {
    if (LeftValue >= RightValue) {
      Angle = ((float)(RightValue/LeftValue) * 90);
    } else if (LeftValue < RightValue) {
      Angle = (180 - ((float)(LeftValue / RightValue)) * 90);
    }
    hbot.Drive(Angle);
  }
}

void loop()
{

  switch (hbot.GetControlMode()) {
    case E_LIGHT_SEEKING_MODE:
      HandleLightSeeking();
      if (count++ > 50)
      {
        hbot.SendPhotoresistorData();
        count = 0;
      }
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
