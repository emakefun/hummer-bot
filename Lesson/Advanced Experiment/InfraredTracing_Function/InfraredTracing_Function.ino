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
  int left1, centre, right1;
  left1 = digitalRead(A0);
  centre = digitalRead(A1);
  right1 = digitalRead(A2);

  if ((right1 == 1) && (centre == 0) && (left1 == 1)) //*******直行*******//
  {
    analogWrite(IN1_PIN, 0);
    analogWrite(IN2_PIN, 180);//the speed value of motorA is val
    analogWrite(IN3_PIN, 180);//the speed value of motorB is val
    analogWrite(IN4_PIN, 0);
  }
  else  if ((right1 == 0) && (centre == 1) && (left1 == 1)) //***左偏,右转***//
  {
    analogWrite(IN1_PIN, 0);
    analogWrite(IN2_PIN, 180);//the speed value of motorB is val
    analogWrite(IN3_PIN, 20);//the speed value of motorB is val
    analogWrite(IN4_PIN, 0);
  }

  else  if ((right1 == 1) && (centre == 1) && (left1 == 0)) //***右偏，左转***//
  {
    analogWrite(IN1_PIN, 0);
    analogWrite(IN2_PIN, 20);//the speed value of motorB is val
    analogWrite(IN3_PIN, 180);//the speed value of motorB is val
    analogWrite(IN4_PIN, 0);
  }

  if ((right1 == 0) && (centre == 0) && (left1 == 0)) //*******停止*******//
  {
    analogWrite(IN1_PIN, 0);
    analogWrite(IN2_PIN, 0);
    analogWrite(IN3_PIN, 0);
    analogWrite(IN4_PIN, 0);
  }
}
