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

#define IR_TRACING_PIN1 A2
#define IR_TRACING_PIN2 A1
#define IR_TRACING_PIN3 A0

ProtocolParser *mProtocol = new ProtocolParser();
Hummerbot hbot(mProtocol, IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN);
static byte count = 0;

void setup()
{
  Serial.begin(9600);
  hbot.init();
  hbot.SetControlMode(E_INFRARED_TRACKING_MODE);
  hbot.SetInfraredTracingPin(IR_TRACING_PIN1, IR_TRACING_PIN2, IR_TRACING_PIN3);
  hbot.SetRgbUltrasonicPin(UL_SING_PIN, UL_RGB_PIN, SERVO_PIN);
  hbot.SetSpeed(0);
}
//**********************************************************************************************
//Name : HandleInfraredTracing
//Function : Infrared Tracing
//Parameter:null
//**********************************************************************************************
void HandleInfraredTracing(void)
{
  static byte old;
  switch (hbot.mInfraredTracing->GetValue()) {
    case IT_ALL_BLACK:
      hbot.KeepStop();
      break;
    case IT_ALL_WHITE:
      hbot.SendInfraredTracking();
      if (old == IT_RIGHT1) {
        while (hbot.mInfraredTracing->GetValue() == IT_ALL_WHITE) {
          hbot.SetSpeed(80);
          hbot.Drive(180);
        }
        old = 0;
        break;
      } if (old == IT_LEFT1) {
        while (hbot.mInfraredTracing->GetValue() == IT_ALL_WHITE) {
          hbot.SetSpeed(80);
          hbot.Drive(0);
        }
        old = 0;
        break;
      }  if (old == IT_RIGHT2) {
        while (hbot.mInfraredTracing->GetValue() == IT_ALL_WHITE) {
          hbot.SetSpeed(50);
          hbot.Drive(160);
        }
        old = 0;
        break;
      } if (old == IT_LEFT2) {
        while (hbot.mInfraredTracing->GetValue() == IT_ALL_WHITE) {
          hbot.SetSpeed(50);
          hbot.Drive(20);
        }
        old = 0;
        break;
      }
      hbot.KeepStop();
      break;
    case IT_CENTER:
      hbot.SetSpeed(45);
      hbot.GoForward();
      break;
    case IT_RIGHT1:
      hbot.SetSpeed(80);
      hbot.Drive(180);
      old = IT_RIGHT1;
      break;
    case IT_RIGHT2:
      hbot.SetSpeed(50);
      hbot.Drive(160);
      old = IT_RIGHT2;
      break;
    case IT_LEFT1:
      hbot.SetSpeed(80);
      hbot.Drive(0);
      old = IT_LEFT1;
      break;
    case IT_LEFT2:
      hbot.SetSpeed(50);
      hbot.Drive(20);
      old = IT_LEFT2;
      break;
  }
}

void loop()
{
  switch (hbot.GetControlMode()) {
    case E_INFRARED_TRACKING_MODE:
    //  DEBUG_LOG(DEBUG_LEVEL_INFO, "E_INFRARED_TRACKING \n");
      HandleInfraredTracing();
      if (count++ > 250)
      {
        hbot.SendInfraredTracking();
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
