#include "IRremote.h"
#include "Keymap.h"

IRremote ir(11);

unsigned char keycode;
char str[128];
void setup()
{
  Serial.begin(9600);
  ir.begin();
}

void loop()
{
  if (keycode = ir.getCode()) {
    String key_name = ir.getKeyMap(keycode, IR_TYPE_EM);
    sprintf(str, "Get ir code: 0x%x key name: %s \n", keycode, (char *)key_name.c_str());
    Serial.println(str);
  } else {
    //   Serial.println("no key");
  }
  delay(110);
}
