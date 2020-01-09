#define IN1_PIN 6   // PWMB
#define IN2_PIN 10  // DIRB  ---  right
#define IN4_PIN 9   // PWMA
#define IN3_PIN 5   // DIRA  ---  left

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
}

void loop()
{
  int left, mid, right;
  left = digitalRead(A0);
  mid = digitalRead(A1);
  right = digitalRead(A2);
  if ((right == 1) && (mid == 0) && (left == 1)) //*******直行*******//
  {
    analogWrite(IN1_PIN, 80);
    analogWrite(IN2_PIN, LOW);//the speed value of motorA is val
    analogWrite(IN3_PIN, LOW);//the speed value of motorB is val
    analogWrite(IN4_PIN, 80);
  }
  else  if ((right == 0) && (mid == 1) && (left == 1)) //***左偏,右转***//
  {
    analogWrite(IN1_PIN, LOW);
    analogWrite(IN2_PIN, 80);//the speed value of motorB is val
    analogWrite(IN3_PIN, LOW);//the speed value of motorB is val
    analogWrite(IN4_PIN, 180);
  }
  else  if ((right == 0) && (mid == 0) && (left == 1)) //***左偏,右转***//
  {
    analogWrite(IN1_PIN, LOW);
    analogWrite(IN2_PIN, 70);//the speed value of motorB is val
    analogWrite(IN3_PIN, LOW);//the speed value of motorB is val
    analogWrite(IN4_PIN, 120);
  }
  else  if ((right == 1) && (mid == 1) && (left == 0)) //***右偏，左转***//
  {
    analogWrite(IN1_PIN, 180);
    analogWrite(IN2_PIN, LOW);//the speed value of motorB is val
    analogWrite(IN3_PIN, 80);//the speed value of motorB is val
    analogWrite(IN4_PIN, LOW);
  }
  
   else  if ((right == 1) && (mid == 0) && (left == 0)) //***右偏，左转***//
  {
    analogWrite(IN1_PIN, 120);
    analogWrite(IN2_PIN, LOW);//the speed value of motorB is val
    analogWrite(IN3_PIN, 70);//the speed value of motorB is val
    analogWrite(IN4_PIN, LOW);
  }
 else if ((right == 0) && (mid == 0) && (left == 0)) //*******停止*******//
  {
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, HIGH);
  }

}
