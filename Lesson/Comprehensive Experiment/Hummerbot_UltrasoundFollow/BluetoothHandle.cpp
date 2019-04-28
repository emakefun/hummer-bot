#include "BluetoothHandle.h"

/****************************************************************************************/
boolean BluetoothHandle::NewButtonState() {
  return ((last_buttons ^ buttons) > 0);
}

/****************************************************************************************/
boolean BluetoothHandle::NewButtonState(unsigned int button) {
  return (((last_buttons ^ buttons) & button) > 0);
}

/****************************************************************************************/
boolean BluetoothHandle::ButtonPressed(unsigned int button) {
  return(NewButtonState(button) & Button(button));
}

/****************************************************************************************/
boolean BluetoothHandle::ButtonReleased(unsigned int button) {
  return((NewButtonState(button)) & ((~last_buttons & button) > 0));
}

/****************************************************************************************/
boolean BluetoothHandle::Button(uint16_t button) {
  return ((~buttons & button) > 0);
}

/****************************************************************************************/
unsigned int BluetoothHandle::ButtonDataByte() {
   return (~buttons);
}

/****************************************************************************************/
byte BluetoothHandle::Analog(byte button) {
   return 1;
}

byte BluetoothHandle::config_gamepad(void) {
   return 1;
}

void BluetoothHandle::SendBluetoothButton(uint16_t button)
{
    byte value[2];
    ST_PROTOCOL SendData;
    SendData.start_code = PROTOCOL_START_CODE;
    SendData.type = E_HELLO_BOT;
    SendData.addr = 0x01;
    SendData.function = E_BATTERY;
    value[0] = (button >> 8) & 0xFF;
    value[1] = button & 0xFF;
    SendData.data = value;
    SendData.len = 8;
    SendData.end_code = PROTOCOL_END_CODE;
   // mProtocolPackage->SendPackage(&SendData, 2);
}
