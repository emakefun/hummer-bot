#include"ServoLib.h"
#define SERVO_PIN 13

Servo_Test mServo(SERVO_PIN);  // Create a servo motor object

char inByte = 0; //Serial port to receive data
int angle = 0;  //Angle value 
String temp = "";//Temporary character variables, or use it for the cache

void setup()   
{  
    Serial.begin(9600);  //Set the baud rate
}

void loop()   
{  
    while (Serial.available() > 0) //Determine whether the serial data
    {  
        inByte = Serial.read();//Read data, the serial port can only read 1 character
        temp += inByte;//The characters read into temporary variables inside the cache, 
        //Continue to determine the serial port there is no data, know all the data read out  
    }
    //Determine whether the temporary variable is empty
    if(temp != "") {  
        angle = temp.toInt();    //Convert variable string type to integer  
        Serial.print("Servo degree: ");
        Serial.println(angle);  //Output data to the serial port for observation
        mServo.SetServoDegree(angle);
     }  
     temp = "";//Please see temporary variables
     delay(100);//Delayed 100 milliseconds 
}  
