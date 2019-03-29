#include "SmartCar.h"
#include "debug.h"

#if ARDUINO > 10609
SmartCar::SmartCar(String name, byte type, byte addr, E_SMARTCAR_CONTROL_MODE control_mode = E_BLUETOOTH_CONTROL)
#else
SmartCar::SmartCar(String name, byte type, byte addr, E_SMARTCAR_CONTROL_MODE control_mode)
#endif
{
    SmartCarName = name;
    SmartCarType = (E_TYPE)type;
    Addr = addr;
    mControlMode = control_mode;
    mStatus = E_STOP;
    BatteryValue = 0;
    Temperature = 0;
    Speed = 0 ;
    Degree = 0;
}

SmartCar::~SmartCar(void)
{

}

#if ARDUINO > 10609
void SmartCar::SetControlMode(E_SMARTCAR_CONTROL_MODE mode=0)
#else
void SmartCar::SetControlMode(E_SMARTCAR_CONTROL_MODE mode)
#endif
{
    if (mode < E_SMARTCAR_CONTROL_MAX && mode >= 0)
    mControlMode = mode;
}

E_SMARTCAR_CONTROL_MODE SmartCar::GetControlMode(void)
{
    return mControlMode;
}

void SmartCar::SetSpeed(int8_t s)
{
    // DEBUG_LOG(DEBUG_LEVEL_INFO, "SetSpeed =%d \n", s);
    if (s > 100) {
        Speed = 100;
        return;
    } else if (s < 0) {
        Speed = 0;
        return;
    }
    Speed = s;
}

int SmartCar::GetSpeed(void)
{
    return Speed;
}

#if ARDUINO > 10609
void SmartCar::SpeedUp(int8_t Duration = 5)
#else
void SmartCar::SpeedUp(int8_t Duration)
#endif
{
    SetSpeed(Speed + Duration);
    mStatus = E_SPEED_UP;
}

#if ARDUINO > 10609
void SmartCar::SpeedDown(int8_t Duration = 5)
#else
void SmartCar::SpeedDown(int8_t Duration )
#endif
{
    SetSpeed(Speed - Duration);
    mStatus = E_SPEED_DOWN;
}

#if ARDUINO > 10609
void SmartCar::SetStatus(E_SMARTCAR_STATUS status=0)
#else
void SmartCar::SetStatus(E_SMARTCAR_STATUS status)
#endif
{
    mStatus = status;
}

E_SMARTCAR_STATUS SmartCar::GetStatus(void)
{
    return mStatus;
}

uint8_t SmartCar::GetBattery(void)
{
    return BatteryValue;
}

uint8_t SmartCar::GetTemperature(void)
{
    return Temperature;
}
