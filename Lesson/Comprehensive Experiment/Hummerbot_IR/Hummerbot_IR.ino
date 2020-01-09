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

ProtocolParser *mProtocol = new ProtocolParser();
Hummerbot hbot(mProtocol, IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN);

void setup()
{
  Serial.begin(9600);
  hbot.init();
  hbot.SetControlMode(E_INFRARED_REMOTE_CONTROL);
  hbot.IrInit();
  hbot.SetRgbUltrasonicPin(UL_SING_PIN, UL_RGB_PIN, SERVO_PIN);
  hbot.SetSpeed(50);
  hbot.mRgbUltrasonic->SetServoBaseDegree(90);
  hbot.mRgbUltrasonic->SetServoDegree(90);
}

void HandleInfaredRemote(byte irKeyCode)
{
  switch ((E_EM_IR_KEYCODE)hbot.mIrRecv->getIrKey(irKeyCode,IR_TYPE_EM)) 
  {
    case EM_IR_KEYCODE_PLUS:
      hbot.SpeedUp(10);
      DEBUG_LOG(DEBUG_LEVEL_INFO, "hbot.Speed = %d \n", hbot.Speed);
      break;
    case EM_IR_KEYCODE_REDUCE:
     DEBUG_LOG(DEBUG_LEVEL_INFO, " start Degree = %d \n", hbot.Degree);
      hbot.SpeedDown(10);
      break;
    case EM_IR_KEYCODE_UP:
      hbot.GoForward();
      break;
    case EM_IR_KEYCODE_DOWN:
      hbot.GoBack();
      break;
    case EM_IR_KEYCODE_OK:
      hbot.KeepStop();
      break;
    case EM_IR_KEYCODE_LEFT:
      hbot.TurnLeft();
      break;
    case EM_IR_KEYCODE_RIGHT:
      hbot.TurnRight();
      break;
    default:
      break;
  }
}


void loop()
{
  switch (hbot.GetControlMode()) {
    
    case E_INFRARED_REMOTE_CONTROL:
      byte irKeyCode;
      if (irKeyCode = hbot.mIrRecv->getCode()) 
      {
      //  Serial.println(irKeyCode);
      //  DEBUG_LOG(DEBUG_LEVEL_INFO, "irKeyCode = %lx \n", irKeyCode);
        HandleInfaredRemote(irKeyCode);
        delay(110);
      }
      else {
        if (hbot.GetStatus() != E_STOP ) {
          hbot.KeepStop();
        }
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
