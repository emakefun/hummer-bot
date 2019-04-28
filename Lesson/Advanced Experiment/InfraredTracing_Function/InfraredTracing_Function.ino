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
  if ((right == 1) && (mid == 0) && (left == 1)) //*******Straight*******//
  {
    analogWrite(IN1_PIN, 0);
    analogWrite(IN2_PIN, 180);//the speed value of motorA is val
    analogWrite(IN3_PIN, 180);//the speed value of motorB is val
    analogWrite(IN4_PIN, 0);
  }
  else  if ((right == 0) && (mid == 1) && (left == 1)) //*** Left, turn right ***//
  {
    analogWrite(IN1_PIN, 0);
    analogWrite(IN2_PIN, 180);//the speed value of motorB is val
    analogWrite(IN3_PIN, 20);//the speed value of motorB is val
    analogWrite(IN4_PIN, 0);
  }
  else  if ((right == 1) && (mid == 1) && (left == 0)) //*** Right, turn left ***//
  {
    analogWrite(IN1_PIN, 0);
    analogWrite(IN2_PIN, 20);//the speed value of motorB is val
    analogWrite(IN3_PIN, 180);//the speed value of motorB is val
    analogWrite(IN4_PIN, 0);
  }
  if ((right == 0) && (mid == 0) && (left == 0)){ //*** stop ***//  
    analogWrite(IN1_PIN, 0);
    analogWrite(IN2_PIN, 0);
    analogWrite(IN3_PIN, 0);
    analogWrite(IN4_PIN, 0);
  }
}
