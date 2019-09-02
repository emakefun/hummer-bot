#include "RGBLed.h"
#define RGB_RED     0xFF0000
#define RGB_GREEN   0x00FF00
#define RGB_BLUE    0x0000FF
#define RGB_YELLOW  0xFFFF00
#define RGB_PURPLE  0xFF00FF
#define RGB_WHITE   0xFFFFFF
const int SingPin = 3;
const int RgbPin = 2;
float distance;
unsigned long Time_Echo_us = 0;
RGBLed mRgb(RgbPin,6);
void setup() {
  Serial.begin(9600);
  Serial.println("Ultrasonic sensor:");
  mRgb.setColor(1,RGB_RED);
  mRgb.setColor(2,RGB_GREEN);
  mRgb.setColor(3,RGB_BLUE);
  mRgb.setColor(4,RGB_YELLOW);
  mRgb.setColor(5,RGB_PURPLE);
  mRgb.setColor(6,RGB_WHITE);
  mRgb.show();
}
void loop() {
  pinMode(SingPin, OUTPUT);
  digitalWrite(SingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(SingPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(SingPin, LOW);
  pinMode(SingPin, INPUT);
 Time_Echo_us = pulseIn(SingPin, HIGH);
    if ((Time_Echo_us < 60000) && (Time_Echo_us > 1)) {
        distance = Time_Echo_us / 58.00;
        Serial.print("distance is :");
        Serial.print(distance);
        Serial.print("cm");
        Serial.println();  
    }
  delay(1000);
}
