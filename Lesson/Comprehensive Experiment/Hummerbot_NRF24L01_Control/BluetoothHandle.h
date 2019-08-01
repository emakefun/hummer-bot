#ifndef _BLUETOOTH_HANDLE_H_
#define _BLUETOOTH_HANDLE_H_

#include "Protocol.h"

//These are our button constants
#define BT_SELECT      0x0001
#define BT_START       0x0002
#define BT_PAD_RIGHT   0x0004
#define BT_PAD_UP      0x0008
#define BT_PAD_LEFT    0x0010
#define BT_PAD_DOWN    0x0020
#define BT_L3          0x0040 
#define BT_R3          0x0080 
#define BT_L2          0x0100
#define BT_R2          0x0200
#define BT_L1          0x0400
#define BT_R1          0x0800
#define BT_GREEN       0x1000
#define BT_RED         0x2000
#define BT_BLUE        0x4000
#define BT_PINK        0x8000
#define BT_TRIANGLE    0x1000
#define BT_CIRCLE      0x2000
#define BT_CROSS       0x4000
#define BT_SQUARE      0x8000

//These are stick values
#define PSS_RX 5
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

#define SET(x,y) (x|=(1<<y))
#define CLR(x,y) (x&=(~(1<<y)))
#define CHK(x,y) (x & (1<<y))
#define TOG(x,y) (x^=(1<<y))

class BluetoothHandle {
  public:
    boolean Button(uint16_t);                //will be TRUE if button is being pressed
    unsigned int ButtonDataByte();
    boolean NewButtonState();
    boolean NewButtonState(unsigned int);    //will be TRUE if button was JUST pressed OR released
    boolean ButtonPressed(unsigned int);     //will be TRUE if button was JUST pressed
    boolean ButtonReleased(unsigned int);    //will be TRUE if button was JUST released
    byte readType(void);
    byte config_gamepad(void);
    byte Analog(byte);
    void SendBluetoothButton(uint16_t button);
  private:
    unsigned char i;
    unsigned int last_buttons;
    unsigned int buttons;
    unsigned long last_read;
    byte read_delay;
    byte controller_type;
    boolean en_Rumble;
    boolean en_Pressures;
};

#endif
