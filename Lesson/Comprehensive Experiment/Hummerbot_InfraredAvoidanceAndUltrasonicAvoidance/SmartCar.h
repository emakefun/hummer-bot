#ifndef _SMART_CAR_H_
#define _SMART_CAR_H_
#include "Arduino.h"
#include "Protocol.h"
#include <stdint.h>

typedef enum
{
    E_BLUETOOTH_CONTROL = 0,
    E_INFRARED_REMOTE_CONTROL,
    E_INFRARED_TRACKING_MODE,
    E_INFRARED_AVOIDANCE_MODE,
    E_ULTRASONIC_AVOIDANCE,
    E_PS2_REMOTE_CONTROL,
    E_NRF24L01_CONTROL,
    E_ULTRASONIC_INFRARED_AVOIDANCE,
    E_PIANO_MODE,
    E_RGB_MODE,
    E_LED_MAXTRIX_MODE,
    E_CMD_LINE_MODE,
    E_LIGHT_SEEKING_MODE,
    E_ULTRASONIC_FOLLOW_MODE,
    E_SMARTCAR_CONTROL_MAX,
} E_SMARTCAR_CONTROL_MODE;

typedef enum
{
    E_FORWARD = 0,
    E_BACK,
    E_LEFT,
    E_RIGHT,
    E_RIGHT_ROTATE,
    E_LEFT_ROTATE,
    E_STOP,
    E_RUNNING,
    E_SPEED_UP,
    E_SPEED_DOWN,
    E_LOW_POWER,
} E_SMARTCAR_STATUS;

class SmartCar {

private :
    String SmartCarName;
    E_TYPE SmartCarType;
    byte Addr;
    E_SMARTCAR_CONTROL_MODE mControlMode;
    E_SMARTCAR_STATUS mStatus;
public :
    SmartCar(String name, byte type, byte addr, E_SMARTCAR_CONTROL_MODE control_mode = E_BLUETOOTH_CONTROL);
    ~SmartCar();
    uint8_t BatteryValue;
    uint8_t Temperature;
    byte Speed ;
    int Degree;
    void GoForward(void);
    void GoBack(void);
    void TurnLeft(void);
    void TurnRight(void);
    void KeepStop(void);
    void Drive(int degree);
    void SetSpeed(int8_t s);
    void SpeedUp(int8_t Duration = 5);
    void SpeedDown(int8_t Duration = 5);
    int  GetSpeed(void);
    void SetControlMode(E_SMARTCAR_CONTROL_MODE mode);
    void SetStatus(E_SMARTCAR_STATUS status);
    E_SMARTCAR_CONTROL_MODE GetControlMode(void);
    E_SMARTCAR_STATUS GetStatus(void);
    uint8_t GetBattery(void);
    uint8_t GetTemperature(void);
};
#endif  // _SMART_CAR_H_
