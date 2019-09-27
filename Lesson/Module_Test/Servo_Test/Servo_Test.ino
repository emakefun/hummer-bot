#include"ServoLib.h"
#define SERVO_PIN 13

Servo_Test mServo(SERVO_PIN);
int degree;

void setup()   
{  
    Serial.begin(9600);  //Set the baud rate
}

void loop()   
{  
    while (Serial.available() > 0) {
  		degree = Serial.parseInt(); // read serial data change to degree
  		if (degree >= 0 && degree <= 180) {
			Serial.print("Servo degree: ");
			Serial.println(degree);
			mServo.SetServoDegree(degree);
  		}
    }
	  delay(10);
}
