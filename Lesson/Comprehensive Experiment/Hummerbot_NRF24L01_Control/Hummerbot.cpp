#include "Hummerbot.h"
#include "ProtocolParser.h"
#include "debug.h"

Hummerbot::Hummerbot(ProtocolParser *Package, uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4): SmartCar("Hummerbot", 0x01, E_BLUETOOTH_CONTROL)
{
  this->In1Pin = in1;
  this->In2Pin = in2;
  this->In3Pin = in3;
  this->In4Pin = in4;
  SetStatus(E_STOP);
  mProtocolPackage = Package;
  Speed = 0;
}

Hummerbot::~Hummerbot()
{
  delete mIrRecv;
  delete mPs2x;
  delete mInfraredTracing;
  delete mPhotoIrAvoidance;
  delete mRgbUltrasonic;
}

#if ARDUINO > 10609
void Hummerbot::Move(int deriction = 1)
#else
void Hummerbot::Move(int deriction)
#endif
{
	if(deriction == 1){
		GoForward();
	}else if(deriction == 2){
		GoBack();
	}else if(deriction == 3){
		TurnLeft();
	}else if(deriction == 4){
		TurnRight();
	}else{
		KeepStop();
	}
}

#if ARDUINO > 10609
void Hummerbot::SetMotorPin(uint8_t in1 = EM_IN1_PIN, uint8_t in2 = EM_IN2_PIN, uint8_t in3 = EM_IN3_PIN, uint8_t in4 = EM_IN4_PIN)
#else
void Hummerbot::SetMotorPin(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4)
#endif
{
  this->In1Pin = in1;
  this->In2Pin = in2;
  this->In3Pin = in3;
  this->In4Pin = in4;
  pinMode(In1Pin, OUTPUT);
  digitalWrite(In1Pin, LOW);
  pinMode(In2Pin, OUTPUT);
  digitalWrite(In2Pin, LOW);
  pinMode(In3Pin, OUTPUT);
  digitalWrite(In3Pin, LOW);
  pinMode(In4Pin, OUTPUT);
  digitalWrite(In4Pin, LOW);
}

void Hummerbot::init(void)
{
  pinMode(In1Pin, OUTPUT);
  digitalWrite(In1Pin, LOW);
  pinMode(In2Pin, OUTPUT);
  digitalWrite(In2Pin, LOW);
  pinMode(In3Pin, OUTPUT);
  digitalWrite(In3Pin, LOW);
  pinMode(In4Pin, OUTPUT);
  digitalWrite(In4Pin, LOW);
}

void Hummerbot::GoForward(void)
{
  int value = (Speed / 10) * 25;
  DEBUG_LOG(DEBUG_LEVEL_INFO, "GoForward\n");
  SetStatus(E_FORWARD);
  analogWrite(In1Pin, value);
  analogWrite(In2Pin, LOW);
  analogWrite(In3Pin, LOW);
  analogWrite(In4Pin, value);
}

void Hummerbot::GoBack(void)
{
  int value = (Speed / 10) * 25;
  DEBUG_LOG(DEBUG_LEVEL_INFO, "GoBack\n");
  SetStatus(E_BACK);
  analogWrite(In1Pin, LOW);
  analogWrite(In2Pin, value);
  analogWrite(In3Pin, value);
  analogWrite(In4Pin, LOW);
}

void Hummerbot::KeepStop(void)
{
  DEBUG_LOG(DEBUG_LEVEL_INFO, "KeepStop\n");
  SetStatus(E_STOP);
  analogWrite(In1Pin, LOW);
  analogWrite(In2Pin, LOW);
  analogWrite(In3Pin, LOW);
  analogWrite(In4Pin, LOW);
}

void Hummerbot::TurnLeft(void)
{
  int value = (Speed / 10) * 25.5; //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
  DEBUG_LOG(DEBUG_LEVEL_INFO, "TurnLeft =%d \n", value);
  analogWrite(In1Pin, value);
  analogWrite(In2Pin, LOW);
  analogWrite(In3Pin, value);
  analogWrite(In4Pin, LOW);
  SetStatus(E_LEFT);
}

void Hummerbot::TurnRight(void)
{
  int value = (Speed / 10) * 25.5; //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
  analogWrite(In1Pin, LOW);
  analogWrite(In2Pin, value);
  analogWrite(In3Pin, LOW);
  analogWrite(In4Pin, value);
  SetStatus(E_RIGHT);
}
void Hummerbot::Drive(void)
{
  Drive(Degree);
}

#if ARDUINO > 10609
void Hummerbot::Drive(int degree = 90)
#else
void Hummerbot::Drive(int degree)
#endif
{
  Degree = degree;
  DEBUG_LOG(DEBUG_LEVEL_INFO, "degree = %d speed = %d\n", degree, Speed);
  int value = (Speed / 10) * 25.5;	 //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
  float f;
  if (degree >= 0 && degree <= 90) {
    f = (float)(degree) / 90;
    analogWrite(In1Pin, (float)(value * f));
    analogWrite(In2Pin, LOW);
    analogWrite(In3Pin, LOW);
    analogWrite(In4Pin, value);
    DEBUG_LOG(DEBUG_LEVEL_INFO, "TurnRight\n");
    SetStatus(E_RIGHT);
  } else if (degree > 90 && degree <= 180) {
    f = (float)(180 - degree) / 90;
    analogWrite (In1Pin, value);
    analogWrite(In2Pin, LOW);
    analogWrite(In3Pin, LOW);
    analogWrite(In4Pin, (float)(value * f));
    SetStatus(E_LEFT);
  } else if (degree > 180 && degree <= 270) {
    f = (float)(degree - 180) / 90;
    analogWrite(In1Pin, LOW);
    analogWrite(In2Pin, value);
    analogWrite(In3Pin, (float)(value * f));
    analogWrite(In4Pin, LOW);
    DEBUG_LOG(DEBUG_LEVEL_INFO, "TurnLeft\n");
    SetStatus(E_LEFT);
  } else if (degree >= 270 && degree <= 360) {
    f = (float)(360 - degree) / 90;
    analogWrite(In1Pin, LOW);
    analogWrite(In2Pin, (float)(value * f));
    analogWrite(In3Pin, value);
    analogWrite(In4Pin, LOW);
    DEBUG_LOG(DEBUG_LEVEL_INFO, "TurnRight\n");
    SetStatus(E_RIGHT);
  }
  else {
    KeepStop();
  }
}


void Hummerbot::IrInit(void )
{
  IrPin = EM_IR_PIN;
  mIrRecv = new IRremote (IrPin);
  mIrRecv->begin();  // Initialize the infrared receiver
}

#if ARDUINO > 10609
void Hummerbot::SetRgbColor(E_RGB_INDEX index = E_RGB_ALL, long Color = RGB_WHITE)
#else
void Hummerbot::SetRgbColor(E_RGB_INDEX index, long Color)
#endif
{
  if (mRgbUltrasonic != NULL) {
    if (index == E_RGB_ALL) {
      mRgbUltrasonic->mRgb->setColor(0, Color);
    } else if (index == E_RGB_RIGHT) {
      mRgbUltrasonic->mRgb->setColor(1, Color);
      mRgbUltrasonic->mRgb->setColor(2, Color);
      mRgbUltrasonic->mRgb->setColor(3, Color);
    } else if (index == E_RGB_LEFT) {
      mRgbUltrasonic->mRgb->setColor(4, Color);
      mRgbUltrasonic->mRgb->setColor(5, Color);
      mRgbUltrasonic->mRgb->setColor(6, Color);
    }
    mRgbUltrasonic->mRgb->show();
  }
}

void Hummerbot::SetRgbEffect(E_RGB_INDEX index, long Color, uint8_t effect)
{
    if (mRgbUltrasonic != NULL) {
        switch((E_RGB_EFFECT)effect) {
            case E_EFFECT_BREATHING:
                for (long i = 0; i < 256; i++) {
                    SetRgbColor(index, (i<<16)|(i<<8)|i);
                    delay((i < 18) ? 18: (256/i));
                }
                for (long i = 255; i >= 0; i--) {
                    SetRgbColor(index, (i<<16)|(i<<8)|i);
                    delay((i < 18) ? 18: (256/i));
                }
                break;
            case E_EFFECT_ROTATE:
                SetRgbColor(E_RGB_ALL, 0);
                mRgbUltrasonic->mRgb->setColor(1, Color);
                mRgbUltrasonic->mRgb->setColor(4, Color);
                mRgbUltrasonic->mRgb->show();
                delay(200);
                mRgbUltrasonic->mRgb->setColor(1, 0);
                mRgbUltrasonic->mRgb->setColor(4, 0);
                mRgbUltrasonic->mRgb->setColor(2, Color);
                mRgbUltrasonic->mRgb->setColor(5, Color);
                mRgbUltrasonic->mRgb->show();
                delay(200);
                mRgbUltrasonic->mRgb->setColor(2, 0);
                mRgbUltrasonic->mRgb->setColor(5, 0);
                mRgbUltrasonic->mRgb->setColor(3, Color);
                mRgbUltrasonic->mRgb->setColor(6, Color);
                mRgbUltrasonic->mRgb->show();
                delay(200);
                mRgbUltrasonic->mRgb->setColor(3, 0);
                mRgbUltrasonic->mRgb->setColor(6, 0);
                mRgbUltrasonic->mRgb->show();
                break;
            case E_EFFECT_FLASH:
                for (byte i = 0; i < 6; i++) {
                   SetRgbColor(E_RGB_ALL, Color);
                   delay(100);
                   SetRgbColor(E_RGB_ALL, 0);
                   delay(100);
                }
                break;
        }
    }
}

#if ARDUINO > 10609
void Hummerbot::SetInfraredTracingPin(uint8_t Pin1 = EM_INFRARED_TRACING_PIN1, uint8_t Pin2 = EM_INFRARED_TRACING_PIN2, uint8_t Pin3 = EM_INFRARED_TRACING_PIN3)
#else
void Hummerbot::SetInfraredTracingPin(uint8_t Pin1, uint8_t Pin2, uint8_t Pin3)
#endif
{
  static bool InfraredTracingInit = false;
  if (!InfraredTracingInit) {
    InfraredTracingPin1 = Pin1;
    InfraredTracingPin2 = Pin2;
    InfraredTracingPin3 = Pin3;
    mInfraredTracing = new InfraredTracing(InfraredTracingPin1, InfraredTracingPin2, InfraredTracingPin3);
    //mInfraredTracing->begin();
    InfraredTracingInit = true;
  }
}

#if ARDUINO > 10609
int Hummerbot::SetPs2xPin(uint8_t clk = EM_PS2X_CLK, uint8_t cmd = EM_PS2X_CMD, uint8_t att = EM_PS2X_ATT, uint8_t dat = EM_PS2X_DAT)
#else
int Hummerbot::SetPs2xPin(uint8_t clk , uint8_t cmd , uint8_t att , uint8_t dat)
#endif
{
  static bool Ps2xInit = false;
  int error = 0 ;
  if (!Ps2xInit) {
    DEBUG_LOG(DEBUG_LEVEL_INFO, "SetPs2xPin\n");
    Ps2xClkPin = clk;
    Ps2xCmdPin = cmd;
    Ps2xAttPin = att;
    Ps2xDatPin = dat;
    mPs2x = new PS2X();
    //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
    //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
    error = mPs2x->config_gamepad(Ps2xClkPin, Ps2xCmdPin, Ps2xAttPin, Ps2xDatPin, false, false);
    if (error == 1) {
      DEBUG_LOG(DEBUG_LEVEL_ERR, "No controller found, check wiring\n");
    } else if (error == 2) {
      DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller found but not accepting commands\n");
    } else if (error == 3) {
      DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller refusing to enter Pressures mode, may not support it\n");
    } else if (error == 0) {
      DEBUG_LOG(DEBUG_LEVEL_INFO, "Found Controller, configured successful\n");
    }
    Ps2xInit = true;
  }
  return error;
}
int Hummerbot::ResetPs2xPin(void)
{
  int error = mPs2x->config_gamepad(Ps2xClkPin, Ps2xCmdPin, Ps2xAttPin, Ps2xDatPin, false, false);
  if (error == 1) {
    DEBUG_LOG(DEBUG_LEVEL_ERR, "No controller found, check wiring\n");
  } else if (error == 2) {
    DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller found but not accepting commands\n");
  } else if (error == 3) {
    DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller refusing to enter Pressures mode, may not support it\n");
  } else if (error == 0) {
    DEBUG_LOG(DEBUG_LEVEL_INFO, "Found Controller, configured successful\n");
  }
  return error;
}

void Hummerbot::SetNrf24L01Pin(uint8_t ce = HB_NRF24L01_CE, uint8_t csn = HB_NRF24L01_CSN)
{
  DEBUG_LOG(DEBUG_LEVEL_INFO, "SetNrf24L01Pin please connect 10uf Capacitance between VCC and GND\n");
  Nrf24L01CePin = ce;
  Nrf24L01CsnPin = csn;
  mNrf24L01 = new Nrf24l(4, 7);
  mNrf24L01->spi = &MirfHardwareSpi;
  mNrf24L01->init();
  mNrf24L01->setRADDR((uint8_t *)"hummer-bot"); //Set your own address (receiver address) using 5 characters
  mNrf24L01->payload = 12;
  mNrf24L01->channel = 90;             //Set the used channel
  mNrf24L01->config();
  delay(100);
}
void Hummerbot::SetRgbUltrasonicPin(uint8_t Sing_Pin, uint8_t Rgb_Pin , uint8_t Sevo_Pin)
{
  static bool UltrasonicInit = false;
  if (!UltrasonicInit) {
    SingPin = Sing_Pin;
    RgbPin = Rgb_Pin;
    ServoPin = Sevo_Pin;
    mRgbUltrasonic = new RgbUltrasonic(SingPin, RgbPin, ServoPin);
    UltrasonicInit = true;
  }
}

void Hummerbot::SetRgbUltrasonicPin(uint8_t Sing_Pin, uint8_t Rgb_Pin)
{
  static bool UltrasonicInit = false;
  if (!UltrasonicInit) {
    SingPin = Sing_Pin;
    RgbPin = Rgb_Pin;
    mRgbUltrasonic = new RgbUltrasonic(SingPin, RgbPin);
    UltrasonicInit = true;
  }
}

#if ARDUINO > 10609
void Hummerbot::SetServoPin(uint8_t Sevo_Pin = EM_SERVO_PIN)
#else
void Hummerbot::SetServoPin(uint8_t Sevo_Pin)
#endif
{
  static bool ServoInit = false;
  if (!ServoInit) {
    ServoPin = Sevo_Pin;
    mRgbUltrasonic->ServoPIN(ServoPin);
    ServoInit = true;
  }
}
//front 0 left 1 right 2
#if ARDUINO > 10609
uint16_t Hummerbot::GetUltrasonicValue(byte direction = 0)
#else
uint16_t Hummerbot::GetUltrasonicValue(byte direction)
#endif
{
  if (direction == 0) {
    return mRgbUltrasonic->GetUltrasonicFrontDistance();
  } else if (direction == 1) {
    return mRgbUltrasonic->GetUltrasonicLeftDistance();
  } else if (direction == 2) {
    return mRgbUltrasonic->GetUltrasonicRightDistance();
  }
}

#if ARDUINO > 10609
void Hummerbot::SetPhotoInfraredAvoidancePin(uint8_t L_Avoidance_Pin = EM_IR_AVOIDANCE_LEFT_PIN, uint8_t R_Avoidance_Pin = EM_IR_AVOIDANCE_RIGHT_PIN, uint8_t L_Photo_Pin = EM_PHOTO_LEFT_PIN, uint8_t R_Photo_Pin = EM_PHOTO_RIGHT_PIN)
#else
void Hummerbot::SetPhotoInfraredAvoidancePin(uint8_t L_Avoidance_Pin, uint8_t R_Avoidance_Pin, uint8_t L_Photo_Pin, uint8_t R_Photo_Pin)
#endif
{
  static bool InfraredAvoidanceInit = false;
  if (!InfraredAvoidanceInit) {
    LeftIrAvoidance = L_Avoidance_Pin;
    RightIrAvoidance = R_Avoidance_Pin;
    LeftPhotosensitive = L_Photo_Pin;
    RightPhotosensitive = R_Photo_Pin;
    mPhotoIrAvoidance = new InfraredAvoidance(LeftIrAvoidance, RightIrAvoidance, LeftPhotosensitive, RightPhotosensitive);
    InfraredAvoidanceInit = true;
  }
}
#if ARDUINO > 10609
void Hummerbot::SetInfraredAvoidancePin(uint8_t L_Avoidance_Pin = EM_IR_AVOIDANCE_LEFT_PIN, uint8_t R_Avoidance_Pin = EM_IR_AVOIDANCE_RIGHT_PIN)
#else
void Hummerbot::SetInfraredAvoidancePin(uint8_t L_Avoidance_Pin, uint8_t R_Avoidance_Pin)
#endif
{
  static bool InfraredAvoidanceInit = false;
  if (!InfraredAvoidanceInit) {
    LeftIrAvoidance = L_Avoidance_Pin;
    RightIrAvoidance = R_Avoidance_Pin;
    mPhotoIrAvoidance = new InfraredAvoidance(LeftIrAvoidance, RightIrAvoidance);
    InfraredAvoidanceInit = true;
  }
}

#if ARDUINO > 10609
void Hummerbot::SetPhotosensitivePin(uint8_t L_Photo_Pin = EM_PHOTO_LEFT_PIN, uint8_t R_Photo_Pin = EM_PHOTO_RIGHT_PIN)
#else
void Hummerbot::SetPhotosensitivePin(uint8_t L_Photo_Pin, uint8_t R_Photo_Pin)
#endif
{
  static bool PhotosensitiveInit = false;
  if (!PhotosensitiveInit) {
    LeftPhotosensitive = L_Photo_Pin;
    RightPhotosensitive = R_Photo_Pin;
    mPhotoIrAvoidance -> PhotosensitivePin(LeftPhotosensitive, RightPhotosensitive);
    PhotosensitiveInit = true;
  }
}

//left 0 right 1
uint8_t Hummerbot::GetInfraredAvoidanceValue(byte direction)
{
    if (direction == 0) {
        return mPhotoIrAvoidance->GetLeftInfraredAvoidanceValue();
    } else if (direction == 1) {
        return mPhotoIrAvoidance->GetRightInfraredAvoidanceValue();
    } else  if (direction == 2) {
        return mPhotoIrAvoidance->GetInfraredAvoidanceValue();
    }
}

//left 0 right 1
#if ARDUINO > 10609
int Hummerbot::GetPhotosensitive(byte direction = 0)
#else
int Hummerbot::GetPhotosensitive(byte direction)
#endif
{
  if (direction == 0 ) {
    return mPhotoIrAvoidance->GetLeftPhotoresistorValue();
  } else if (direction == 1) {
    return mPhotoIrAvoidance->GetRightPhotoresistorValue();
  }
}
#if ARDUINO > 10609
uint8_t Hummerbot::GetInfraredTracingValue(byte direction = 0)
#else
uint8_t Hummerbot::GetInfraredTracingValue(byte direction)
#endif
{
  if (direction == 0 ) {
    return mInfraredTracing->GetLeftValue();
  } else if (direction == 1) {
    return mInfraredTracing->GetMiddleValue();
  } else if (direction == 2) {
    return mInfraredTracing->GetRightValue();
  }
}

void Hummerbot::SendInfraredTracking(void)
{
  byte InfraredTracking_value = mInfraredTracing->GetValue();
  SendData.start_code = PROTOCOL_START_CODE;
  SendData.type = E_HUMMER_BOT;
  SendData.addr = 0x01;
  SendData.function = E_INFRARED_TRACKING;
  SendData.data = &InfraredTracking_value;
  SendData.len = 8;
  SendData.end_code = PROTOCOL_END_CODE;
  mProtocolPackage->SendPackage(&SendData, 1);
}

void Hummerbot::SendInfraredAvoidanceData(void)
{
  byte InfraredAvoidance_value = (((byte)mPhotoIrAvoidance->GetLeftInfraredAvoidanceValue() & 0xFF) << 1) | ((byte)mPhotoIrAvoidance->GetRightPhotoresistorValue() & 0xFF);
  SendData.start_code = PROTOCOL_START_CODE;
  SendData.type = E_HUMMER_BOT;
  SendData.addr = 0x01;
  SendData.function = E_INFRARED_AVOIDANCE;
  SendData.data = &InfraredAvoidance_value;
  SendData.len = 8;
  SendData.end_code = PROTOCOL_END_CODE;
  mProtocolPackage->SendPackage(&SendData, 1);
}

void Hummerbot::SendUltrasonicData(void)
{
  union
  {
    float d;
    byte data[4];
  } value;
  value.d = GetUltrasonicValue(FRONT);
  SendData.start_code = 0xAA;
  SendData.type = E_HUMMER_BOT;
  SendData.addr = 0x01;
  SendData.function = E_ULTRASONIC;
  SendData.data = value.data;
  SendData.len = 10;
  SendData.end_code = 0x55;
  mProtocolPackage->SendPackage(&SendData, 4);
}

void Hummerbot::SendPhotoresistorData(void) {
  byte Photoresistor[2];
  SendData.start_code = PROTOCOL_START_CODE;
  SendData.type = E_HUMMER_BOT;
  SendData.addr = 0x01;
  SendData.function = E_PHOTORESISTOR;
  Photoresistor[0] = (mPhotoIrAvoidance->GetLeftPhotoresistorValue() / 10.24);
  Photoresistor[1] = (mPhotoIrAvoidance->GetRightPhotoresistorValue() / 10.24);
  SendData.data = Photoresistor;
  SendData.len = 8;
  SendData.end_code = PROTOCOL_END_CODE;
  mProtocolPackage->SendPackage(&SendData, 2);
}

void Hummerbot::SendVersionPackage(void)
{
  char value[5];
  SendData.start_code = PROTOCOL_START_CODE;
  SendData.type = E_HUMMER_BOT;
  SendData.addr = 0x01;
  SendData.function = E_VERSION;
  SendData.data = (byte*)value;
  SendData.len = 13;
  SendData.end_code = PROTOCOL_END_CODE;
  mProtocolPackage->SendPackage(&SendData, 5);
}