#include <Servo.h>
Servo myservo;
int ServoPin = 13;
void setup()
{
  Serial.begin(9600);  //Set the baud rate
  pinMode(ServoPin, OUTPUT);
  myservo.attach(ServoPin);
}
void loop()
{
   while(Serial.available()) {
    int servo_angle = Serial.parseInt();
    if (servo_angle >= 0 && servo_angle <= 180) {
      myservo.write(servo_angle);
      Serial.print("Servo degree: ");
      Serial.println(servo_angle);
    //Control the servo to rotate the corresponding angle.
    }
  }
  delay(100);//Delayed 100 milliseconds
}
