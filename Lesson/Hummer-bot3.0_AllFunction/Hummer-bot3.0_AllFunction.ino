#include "Hummerbot.h"
#include "BluetoothHandle.h"
#include "ProtocolParser.h"
#include "KeyMap.h"
#include "debug.h"

#define IN1_PIN 6   // PWMB
#define IN2_PIN 10  // DIRB  ---  right
#define IN3_PIN 5   // DIRA  ---  left
#define IN4_PIN 9   // PWMA

#define IR_PIN 12
#define SERVO_PIN 13
#define UL_SING_PIN 3
#define UL_RGB_PIN 2
#define PS2X_CLK 11
#define PS2X_CMD 7
#define PS2X_CS  8
#define PS2X_DAT 4
#define IR_TRACING_PIN1 A2
#define IR_TRACING_PIN2 A1
#define IR_TRACING_PIN3 A0

#define PHOTOSENSITIVE_LEFT_PIN A2
#define PHOTOSENSITIVE_RIGHT_PIN A4
#define IR_AVOIDANCE_LEFT_PIN A3
#define IR_AVOIDANCE_RIGHT_PIN A5

ProtocolParser *mProtocol = new ProtocolParser();
Hummerbot hbot(mProtocol, IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN);
byte Ps2xStatus, Ps2xType;

void setup()
{
  Serial.begin(9600);
  hbot.init();
  hbot.SetControlMode(E_INFRARED_TRACKING_MODE);//E_BLUETOOTH_CONTROL/E_INFRARED_TRACKING_MODE
  hbot.IrInit();
  hbot.SetRgbUltrasonicPin(UL_SING_PIN, UL_RGB_PIN, SERVO_PIN);
  hbot.SetPs2xPin(PS2X_CLK, PS2X_CMD, PS2X_CS, PS2X_DAT);
  hbot.SetSpeed(0);
  Ps2xType = hbot.mPs2x->readType();
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
  DEBUG_LOG(DEBUG_LEVEL_INFO, "UlFrontDistance =%d \n", UlFrontDistance);
  delay(20);
  if ((RightValue != IA_THRESHOLD) && (LeftValue == IA_THRESHOLD)) {
    hbot.SetSpeed(70);
    hbot.Drive(20);
  } else if ((RightValue == IA_THRESHOLD) && (LeftValue != IA_THRESHOLD)) {
    hbot.SetSpeed(70);
    hbot.Drive(160);
  } else {
    hbot.SetSpeed(45);
    hbot.GoForward();
  }
  DEBUG_LOG(DEBUG_LEVEL_INFO, "UlFrontDistance = %d \n\r", UlFrontDistance);
  if (UlFrontDistance < UL_LIMIT_MID) {
    hbot.KeepStop();
    if (UlFrontDistance <= UL_LIMIT_MIN || (RightValue == IA_THRESHOLD && LeftValue == IA_THRESHOLD)) {
      hbot.SetSpeed(60);
      hbot.GoBack();
      delay(300);
      hbot.KeepStop();
    }
    UlLeftDistance = hbot.GetUltrasonicValue(LEFT);
    UlRightDistance = hbot.GetUltrasonicValue(RIGHT);
    if (UlRightDistance >= UlLeftDistance) {
      hbot.SetSpeed(100);
      hbot.TurnRight();
      delay(400);
    }
    if (UlLeftDistance > UlRightDistance) {
      hbot.SetSpeed(100);
      hbot.TurnLeft();
      delay(400);
    }
    if (UlLeftDistance <= UL_LIMIT_MIN && UlRightDistance <= UL_LIMIT_MIN ) {
      hbot.SetSpeed(100);
      hbot.Drive(0);
      delay(800);
      hbot.KeepStop();
    }
    hbot.KeepStop();
  }
}

void HandleUltrasonicAvoidance(void)
{
  uint16_t UlFrontDistance, UlLeftDistance, UlRightDistance;
  UlFrontDistance =  hbot.GetUltrasonicValue(FRONT);
  DEBUG_LOG(DEBUG_LEVEL_INFO, "UlFrontDistance =%d \n", UlFrontDistance);
  if ((UlFrontDistance < UL_LIMIT_MIN))
  {
    hbot.SetSpeed(100);
    hbot.GoBack();
    delay(250);
  }
  if (UlFrontDistance < UL_LIMIT_MID)
  {
    hbot.KeepStop();
    delay(100);
    UlLeftDistance = hbot.GetUltrasonicValue(LEFT);
    UlRightDistance = hbot.GetUltrasonicValue(RIGHT);
    if ((UlRightDistance > UL_LIMIT_MIN) && (UlRightDistance < UL_LIMIT_MAX))
    {
      hbot.SetSpeed(80);
      hbot.TurnRight();
      delay(310);
    }
    else if ((UlLeftDistance > UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MAX))
    {
      hbot.SetSpeed(80);
      hbot.TurnLeft();
      delay(310);
    }
    else if ((UlRightDistance < UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MIN) )
    {
      hbot.SetSpeed(80);
      hbot.Drive(0);
      delay(510);
    }
  } else {
    hbot.SetSpeed(80);
    hbot.GoForward();
  }
}
//=============================Infrared tracking
void HandleInfraredTracing(void)
{
  static byte old;
  switch (hbot.mInfraredTracing->GetValue()) {
    case IT_ALL_BLACK:
      hbot.KeepStop();
      break;
    case IT_ALL_WHITE:
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
          hbot.SetSpeed(60);
          hbot.Drive(160);
        }
        old = 0;
        break;
      } if (old == IT_LEFT2) {
        while (hbot.mInfraredTracing->GetValue() == IT_ALL_WHITE) {
          hbot.SetSpeed(60);
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
      hbot.SetSpeed(60);
      hbot.Drive(160);
      old = IT_RIGHT2;
      break;
    case IT_LEFT1:
      hbot.SetSpeed(80);
      hbot.Drive(0);
      old = IT_LEFT1;
      break;
    case IT_LEFT2:
      hbot.SetSpeed(60);
      hbot.Drive(20);
      old = IT_LEFT2;
      break;
  }
}
//========================= bluetooth
void HandleBluetoothRemote(bool recv_flag)
{
  if (recv_flag) {
    switch (mProtocol->GetRobotControlFun()) {
      case E_BUTTON:
        switch (mProtocol->GetBluetoothButton())
        {
          case BT_PAD_UP:
            hbot.GoForward();
            break;
          case BT_PAD_DOWN:
            hbot.GoBack();
            break;
          case BT_PAD_LEFT:
            hbot.TurnLeft();
            break;
          case BT_PAD_RIGHT:
            hbot.TurnRight();
            break;
        }
        break;
      case E_ROBOT_CONTROL_DIRECTION:
        hbot.Drive(mProtocol->GetRobotDegree());
        break;
      case E_ROBOT_CONTROL_SPEED:
        hbot.SetSpeed(mProtocol->GetRobotSpeed());
        break ;
      case E_CONTROL_MODE:
        //Serial.println(mProtocol->GetControlMode());
        hbot.SetControlMode(mProtocol->GetControlMode());
        break;
      case E_LED:
        hbot.SetRgbColor(E_RGB_ALL, mProtocol->GetRgbValue());
        break;
      case E_VERSION:
        hbot.SendVersionPackage();
        break;
    }
  }
}

void HandleInfaredRemote(byte irKeyCode)
{
  switch ((E_IR_KEYCODE)hbot.mIrRecv->getIrKey(irKeyCode)) {
    case IR_KEYCODE_PLUS:
      hbot.SpeedUp(10);
      DEBUG_LOG(DEBUG_LEVEL_INFO, "hbot.Speed = %d \n", hbot.Speed);
      break;
    case IR_KEYCODE_REDUCE:
      DEBUG_LOG(DEBUG_LEVEL_INFO, " start Degree = %d \n", hbot.Degree);
      hbot.SpeedDown(10);
      break;
    case IR_KEYCODE_UP:
      hbot.GoForward();
      break;
    case IR_KEYCODE_DOWN:
      hbot.GoBack();
      break;
    case IR_KEYCODE_OK:
      hbot.KeepStop();
      break;
    case IR_KEYCODE_LEFT:
      hbot.TurnLeft();
      break;
    case IR_KEYCODE_RIGHT:
      hbot.TurnRight();
      break;
    default:
      break;
  }
}

//====================================InfraredAvoidance
void HandleInfraredAvoidance()
{
  switch (hbot.GetInfraredAvoidanceValue(0)) {
    case IOA_ALL_NO:
      hbot.SetSpeed(70);
      hbot.GoForward();
      break;
    case IOA_ALL_YES:
      hbot.SetSpeed(70);
      hbot.GoBack();
      break;
    case IOA_LEFT:
      hbot.SetSpeed(80);
      hbot.TurnRight();
      break;
    case IOA_RIGHT:
      hbot.SetSpeed(80);
      hbot.TurnLeft();
      break;
    default:
      break;
  }
}

void HandleLightFinding()
{
  float LeftValue, RightValue;
  int Angle;
  hbot.SetSpeed(80);
  LeftValue = hbot.GetPhotosensitive(0) / 10;
  RightValue = hbot.GetPhotosensitive(1) / 10;
  if ((LeftValue > 30) && (RightValue > 30))
  {
    hbot.KeepStop();
  } else {
    if (LeftValue >= RightValue) {
      Angle = ((float)(RightValue / LeftValue) * 90);
    } else if (LeftValue < RightValue) {
      Angle = (180 - ((float)(LeftValue / RightValue)) * 90);
    }
    hbot.Drive(Angle);
  }
}

void HandlePS2()
{
  static int vibrate = 0;
  byte PSS_X = 0, PSS_Y = 0;
  hbot.mPs2x->read_gamepad(false, vibrate); // read controller and set large motor to spin at 'vibrate' speed
  if (hbot.mPs2x->ButtonDataByte()) {
    if (hbot.mPs2x->Button(PSB_PAD_UP)) {     //will be TRUE as long as button is pressed
      hbot.GoForward();
    }
    if (hbot.mPs2x->Button(PSB_PAD_RIGHT)) {
      hbot.Drive(20);
    }
    if (hbot.mPs2x->Button(PSB_PAD_LEFT)) {
      hbot.Drive(160);
    }
    if (hbot.mPs2x->Button(PSB_PAD_DOWN)) {
      hbot.GoBack();
    }
    vibrate = hbot.mPs2x->Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
    if (hbot.mPs2x->Button(PSB_CROSS)) {             //will be TRUE if button was JUST pressed OR released
      hbot.SpeedDown(5);
    }
    if (hbot.mPs2x->Button(PSB_TRIANGLE)) {
      hbot.SpeedUp(5);
    }
    if (hbot.mPs2x->Button(PSB_SQUARE)) {
      hbot.TurnLeft();
    }
    if (hbot.mPs2x->Button(PSB_CIRCLE)) {
      hbot.TurnRight();
    }
    if (hbot.mPs2x->Button(PSB_L1) || hbot.mPs2x->Button(PSB_R1)) {
      uint16_t RightValue = hbot.mPs2x->RightHart();
      uint16_t LeftValue = hbot.mPs2x->LeftHart();
      if (RightValue != CENTER) {
        if ((RightValue > 0) && (RightValue < 180 )) {
          hbot.SpeedUp(2);
        } else {
          hbot.SpeedDown(2);
        }
      }
      if (LeftValue != CENTER) {
        hbot.Drive(LeftValue);
      } else {
        hbot.KeepStop();
      }
    }
  }
  else {
    hbot.KeepStop();
  }
  delay(50);
}

void loop()
{
  static byte mode;
  static bool recv_flag;
  mProtocol->RecevData();
  if (recv_flag = mProtocol->ParserPackage()) {
    if (mProtocol->GetRobotControlFun() == E_CONTROL_MODE) {
      hbot.SetControlMode(mProtocol->GetControlMode());
      return;
    }
  }
  switch (hbot.GetControlMode()) {
    case E_BLUETOOTH_CONTROL:
      HandleBluetoothRemote(recv_flag);
      DEBUG_LOG(DEBUG_LEVEL_INFO, "E_BLUETOOTH_CONTROL \n");
      break;
    case E_INFRARED_REMOTE_CONTROL:
      byte irKeyCode;
      if (irKeyCode = hbot.mIrRecv->getCode()) {
        DEBUG_LOG(DEBUG_LEVEL_INFO, "irKeyCode = %lx \n", irKeyCode);
        HandleInfaredRemote(irKeyCode);
        delay(110);
      }
      else {
        if (hbot.GetStatus() != E_STOP ) {
          hbot.KeepStop();
        }
      }
      break;
    case E_INFRARED_TRACKING_MODE:
      if (mode != E_INFRARED_TRACKING_MODE) {
        hbot.SetInfraredTracingPin(IR_TRACING_PIN1, IR_TRACING_PIN2, IR_TRACING_PIN3);
        mode = E_INFRARED_TRACKING_MODE;
      }
      DEBUG_LOG(DEBUG_LEVEL_INFO, "E_INFRARED_TRACKING \n");
      HandleInfraredTracing();
      hbot.SendInfraredTracking();
      break;
    case E_INFRARED_AVOIDANCE_MODE:
      if (mode != E_INFRARED_AVOIDANCE_MODE) {
        hbot.SetPhotoInfraredAvoidancePin(IR_AVOIDANCE_LEFT_PIN, IR_AVOIDANCE_RIGHT_PIN, PHOTOSENSITIVE_LEFT_PIN, PHOTOSENSITIVE_RIGHT_PIN);
        mode = E_INFRARED_AVOIDANCE_MODE;
      }
      DEBUG_LOG(DEBUG_LEVEL_INFO, "E_INFRARED_AVOIDANCE \n");
      HandleInfraredAvoidance();
      break;
    case E_LIGHT_FINDING_MODE:
      if (mode != E_LIGHT_FINDING_MODE) {
        hbot.SetPhotoInfraredAvoidancePin(IR_AVOIDANCE_LEFT_PIN, IR_AVOIDANCE_RIGHT_PIN, PHOTOSENSITIVE_LEFT_PIN, PHOTOSENSITIVE_RIGHT_PIN);
        mode = E_LIGHT_FINDING_MODE;
      }
      HandleLightFinding();
      break;
    case E_ULTRASONIC_AVOIDANCE:
      DEBUG_LOG(DEBUG_LEVEL_INFO, "E_ULTRASONIC_AVOIDANCE \n");
      HandleUltrasonicAvoidance();
      break;
    case E_ULTRASONIC_INFRARED_AVOIDANCE:
      if ((mode != E_ULTRASONIC_AVOIDANCE) || (mode != E_ULTRASONIC_INFRARED_AVOIDANCE)) {
        hbot.SetPhotoInfraredAvoidancePin(IR_AVOIDANCE_LEFT_PIN, IR_AVOIDANCE_RIGHT_PIN, PHOTOSENSITIVE_LEFT_PIN, PHOTOSENSITIVE_RIGHT_PIN);
        mode = E_ULTRASONIC_INFRARED_AVOIDANCE;
      }
      DEBUG_LOG(DEBUG_LEVEL_INFO, "E_ULTRASONIC_INFRARED_AVOIDANCE \n");
      HandleUltrasonicInfraredAvoidance();
      hbot.SendInfraredAvoidanceData();
      hbot.SendUltrasonicData();
      hbot.SendPhotoresistorData();
      break;
    case E_PS2_REMOTE_CONTROL:
      while (Ps2xStatus != 0) { //skip loop if no controller found
        delay(500);
        Ps2xStatus = hbot.ResetPs2xPin();
        Ps2xType = hbot.mPs2x->readType();
        DEBUG_LOG(DEBUG_LEVEL_INFO, "E_PS2_REMOTE_CONTROL \n");
      }
      if (Ps2xType != 2) {
        HandlePS2();
      }
      break;
    case E_RGB_MODE:
      if (recv_flag) {
        switch (mProtocol->GetRobotControlFun()) {
          case E_LED:
            hbot.SetRgbColor(E_RGB_ALL, mProtocol->GetRgbValue());
            break;
        }
      }
      return;
    default:
      break;
  }
  switch (hbot.GetStatus()) {
    case E_FORWARD:
      hbot.SetRgbColor(E_RGB_ALL, RGB_WHITE);
      break;
    case E_LEFT:
      hbot.SetRgbColor(E_RGB_LEFT, RGB_WHITE);
      break;
    case E_RIGHT:
      hbot.SetRgbColor(E_RGB_RIGHT, RGB_WHITE);
      //   Mirage.Sing(S_OhOoh);
      break;
    case E_BACK:
      hbot.SetRgbColor(E_RGB_ALL, RGB_RED);
      break;
    case E_STOP:
      hbot.SetRgbColor(E_RGB_ALL, RGB_OFF);
      break;
    case E_SPEED_UP:
      hbot.SetRgbColor(E_RGB_ALL, hbot.GetSpeed() * 2.5);
      break;
    case E_SPEED_DOWN:
      hbot.SetRgbColor(E_RGB_ALL, hbot.GetSpeed() * 2.5);
      break;
    default:
      break;
  }
}
