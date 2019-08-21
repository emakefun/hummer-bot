#ifndef _HUMMERBOT_H_
#define _HUMMERBOT_H_
#include <stdint.h>
#include "Arduino.h"
#include "SmartCar.h"
#include "IRremote.h"
#include "PS2X_lib.h"  //for v1.6
#include "InfraredAvoidance.h"
#include "InfraredTracing.h"
#include "ProtocolParser.h"
#include "nRF24L01.h"
#include "RgbUltrasonic.h"

#define EM_IR_PIN 11
#define EM_IN1_PIN 6
#define EM_IN2_PIN 10
#define EM_IN4_PIN 9
#define EM_IN3_PIN 5

#define EM_SING_PIN 3
#define EM_RGB_PIN 2
#define EM_SERVO_PIN 13

#define EM_IR_AVOIDANCE_LEFT_PIN 12
#define EM_IR_AVOIDANCE_RIGHT_PIN A5
#define EM_PHOTO_LEFT_PIN A3
#define EM_PHOTO_RIGHT_PIN A4

#define EM_INFRARED_TRACING_PIN1 A2
#define EM_INFRARED_TRACING_PIN2 A1
#define EM_INFRARED_TRACING_PIN3 A0

#define EM_PS2X_CLK 11
#define EM_PS2X_CMD 7
#define EM_PS2X_ATT 8
#define EM_PS2X_DAT 4

#define HB_NRF24L01_CE 4
#define HB_NRF24L01_CSN 7
#define HB_NRF24L01_MOSI 11
#define HB_NRF24L01_MISO 12
#define HB_NRF24L01_SCK 13
#define FRONT 0
#define LEFT 1
#define RIGHT 2
#define MIDDLE 3

typedef enum
{
    E_RGB_ALL = 0,
    E_RGB_RIGHT = 1,
    E_RGB_LEFT = 2
} E_RGB_INDEX;

typedef enum
{
    E_EFFECT_BREATHING = 0,
    E_EFFECT_ROTATE = 1,
    E_EFFECT_FLASH = 2
} E_RGB_EFFECT;

class Hummerbot : public SmartCar {
private :
    uint8_t In1Pin, In2Pin, PwmaPin, In3Pin, In4Pin;
    uint8_t IrPin;      // Infrared remoter pin
    uint8_t InfraredTracingPin1, InfraredTracingPin2, InfraredTracingPin3, InfraredTracingPin4, InfraredTracingPin5;    // for Infrared tracing pin
    uint8_t Ps2xClkPin, Ps2xCmdPin, Ps2xAttPin, Ps2xDatPin;    // for Ps2 remoter
    uint8_t LeftIrAvoidance, RightIrAvoidance;     //For infrared obstacle avoidance
    uint8_t LeftPhotosensitive, RightPhotosensitive;    
    uint8_t SingPin,RgbPin, ServoPin;
    uint8_t Nrf24L01CePin, Nrf24L01CsnPin;    // for nRF24L01 control
    ST_PROTOCOL SendData;
    ProtocolParser *mProtocolPackage;

public :
    Hummerbot(ProtocolParser *Package, uint8_t in1 = EM_IN1_PIN, uint8_t in2 = EM_IN2_PIN, uint8_t in3 = EM_IN3_PIN, uint8_t in4 = EM_IN4_PIN);
    ~Hummerbot();
    IRremote  *mIrRecv;
    PS2X *mPs2x;
    Nrf24l *mNrf24L01;
    InfraredTracing *mInfraredTracing;
    InfraredAvoidance *mPhotoIrAvoidance;
    RgbUltrasonic *mRgbUltrasonic;
    void GoForward(void);
    void GoBack(void);
    void TurnLeft(void);
    void TurnRight(void);
    void KeepStop(void);
    void Drive(void);
	void Move(int direction);
    void Drive(int degree);
    void IrInit(void);
    void SetRgbColor(E_RGB_INDEX index = E_RGB_ALL, long Color = RGB_WHITE);
    void SetRgbEffect(E_RGB_INDEX index, long Color, uint8_t effect);
	void SetRgbUltrasonicPin(uint8_t Sing_Pin, uint8_t Rgb_Pin);
    void SetServoPin(uint8_t Sevo_Pin = EM_SERVO_PIN);
	void SetInfraredAvoidancePin(uint8_t L_Avoidance_Pin = EM_IR_AVOIDANCE_LEFT_PIN, uint8_t R_Avoidance_Pin = EM_IR_AVOIDANCE_RIGHT_PIN);
    void SetPhotosensitivePin(uint8_t L_Photo_Pin = EM_PHOTO_LEFT_PIN, uint8_t R_Photo_Pin = EM_PHOTO_RIGHT_PIN);
	void SetInfraredTracingPin(uint8_t Pin1 = EM_INFRARED_TRACING_PIN1, uint8_t Pin2 = EM_INFRARED_TRACING_PIN2, uint8_t Pin3 = EM_INFRARED_TRACING_PIN3);
	int SetPs2xPin(uint8_t clk = EM_PS2X_CLK, uint8_t cmd = EM_PS2X_CMD, uint8_t att = EM_PS2X_ATT, uint8_t dat = EM_PS2X_DAT);
	int ResetPs2xPin(void);
    void SetNrf24L01Pin(uint8_t ce = HB_NRF24L01_CE, uint8_t csn = HB_NRF24L01_CSN);	
	uint16_t GetUltrasonicValue(byte);//front 0 left 1 right 2
    uint8_t GetInfraredAvoidanceValue(byte); //left 0 right 1
    int GetPhotosensitive(byte); //left 0 right 1
    uint8_t GetInfraredTracingValue(byte);
    
    void SendBatteryPackage(byte *battery_value);
    void SendInfraredTracking(void);
    void SendInfraredAvoidanceData(void);
    void SendUltrasonicData(void);
    void SendPhotoresistorData(void);
    void SendVersionPackage(void);
    void init(void);
	
	void SetMotorPin(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4);
    void SetRgbUltrasonicPin(uint8_t Sing_Pin , uint8_t Rgb_Pin, uint8_t Sevo_Pin);
    void SetPhotoInfraredAvoidancePin(uint8_t L_Avoidance_Pin = EM_IR_AVOIDANCE_LEFT_PIN, uint8_t R_Avoidance_Pin = EM_IR_AVOIDANCE_RIGHT_PIN,uint8_t L_Photo_Pin = EM_PHOTO_LEFT_PIN, uint8_t R_Photo_Pin = EM_PHOTO_RIGHT_PIN);
};

#endif  /* _HUMMERBOT_H_ */