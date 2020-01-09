#include "RgbUltrasonic.h"
#define IN2_PIN 10 // PWMB
#define IN1_PIN 6  // DIRB  ---  right
#define IN4_PIN 9  // PWMA
#define IN3_PIN 5  // DIRA  ---  left
#define SERVO_PIN 13
#define SING_PIN 3
#define RGB_PIN 2
#define UL_LIMIT_MID 20
#define UL_LIMIT_MAX 550
RgbUltrasonic mRgbUltrasonic(SING_PIN, RGB_PIN, SERVO_PIN);/*Define ultrasonic and servo pins*/

void setup()
{
  Serial.begin(9600);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  mRgbUltrasonic.SetServoBaseDegree(90);/*Adjust the initial angle of the steering gear according to the steering gear error*/
  mRgbUltrasonic.SetServoDegree(90);/*Set the servo angle*/
}

void loop()
{
  uint16_t FrontDistance, LeftDistance, RightDistance;
  FrontDistance = mRgbUltrasonic.GetUltrasonicFrontDistance();/*The ultrasonic module collects the front data*/
  delay(50);
  if ((FrontDistance > UL_LIMIT_MID) && (FrontDistance < UL_LIMIT_MAX))
  /*According to the data collected by the ultrasonic module and the infrared obstacle avoidance module,
    it is judged whether there is an obstacle in front, and if there is no obstacle, go straight.*/
  {
    analogWrite(IN1_PIN, 100);
    analogWrite(IN2_PIN, LOW);
    analogWrite(IN3_PIN, LOW);
    analogWrite(IN4_PIN, 100);
  } 
  else if ((FrontDistance < UL_LIMIT_MID) || (FrontDistance > UL_LIMIT_MAX))
  /*According to the data collected by the ultrasonic module and the infrared obstacle avoidance module, it is determined whether there is an obstacle in front. For example, 
   * the infrared obstacle avoidance module determines that there is no obstacle in front, and the ultrasonic module determines that the right obstacle is an obstacle, 
   * first stops the car, and uses the ultrasonic module to perform left and right.*/
  {
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, HIGH);
    RightDistance = mRgbUltrasonic.GetUltrasonicRightDistance();/*The ultrasonic module collects the right side*/
    LeftDistance = mRgbUltrasonic.GetUltrasonicLeftDistance();/*The ultrasonic module collects the left side*/    
    delay(10);
    if ((RightDistance > UL_LIMIT_MID) && (RightDistance < UL_LIMIT_MAX) && (RightDistance > LeftDistance))
    /*According to the ultrasonic module to collect the data on the left and right sides to determine whether there is an obstacle on the right side.*/
    {
      analogWrite(IN1_PIN, LOW);
      analogWrite(IN2_PIN, 200);
      analogWrite(IN3_PIN, LOW);
      analogWrite(IN4_PIN, 200);
      Serial.println("testRight");
      delay(380);
    }
    else if ((LeftDistance > UL_LIMIT_MID) && (LeftDistance < UL_LIMIT_MAX) && (LeftDistance > RightDistance))
   /*According to the ultrasonic module to collect the data on the left and right sides to determine whether there is an obstacle on the left side.*/
    {
      analogWrite(IN1_PIN, 200);
      analogWrite(IN2_PIN, LOW);
      analogWrite(IN3_PIN, 200);
      analogWrite(IN4_PIN, LOW);
      Serial.println("testLeft");
      delay(380);
    }
    else if ((RightDistance < UL_LIMIT_MID) && (LeftDistance < UL_LIMIT_MID) )
    /*According to the ultrasonic module to collect the data on the left and right sides to determine whether there are obstacles on the left and right sides*/
    {
      analogWrite(IN1_PIN, 220);
      analogWrite(IN2_PIN, 0);
      analogWrite(IN3_PIN, 220);
      analogWrite(IN4_PIN, 0);
      delay(760);
    }
  }
}
