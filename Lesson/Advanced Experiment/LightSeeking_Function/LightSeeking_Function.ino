#define IN1_PIN 6   // PWMB
#define IN2_PIN 10  // DIRB  ---  right
#define IN4_PIN 9   // PWMA
#define IN3_PIN 5   // DIRA  ---  left
const int leftPin = A2;
const int rightPin = A4;
float LeftValue,RightValue;
int Angle;
float f;
//使用此程序时请把红外寻迹模块的连接线给拔了，只需要拔连接扩展板的一头就行了
void setup()
{
  Serial.begin(9600);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
  delay(1000);
}

void loop()
{
  LeftValue = analogRead(leftPin) / 10;
  RightValue = analogRead(rightPin) / 10;
  if ( (LeftValue > 50) && (RightValue > 50)) {
    analogWrite(IN1_PIN, LOW); //the speed value of motorA is val
    analogWrite(IN2_PIN, LOW);
    analogWrite(IN3_PIN, LOW);
    analogWrite(IN4_PIN, LOW); //the speed value of motorB is val
  } else {
    if (LeftValue > RightValue) {
      Angle = ((float)(RightValue/LeftValue)) * 90;
    } else if (LeftValue < RightValue) {
      Angle = (90 - ((float)(LeftValue/RightValue)) * 90) + 90;
    }
    Serial.println(Angle);
    if (Angle <= 90) {
      f = (float)(Angle) / 90;
      analogWrite(IN1_PIN, (float)(200 * f)); //the speed value of motorA is val
      analogWrite(IN2_PIN, LOW);
      analogWrite(IN3_PIN, LOW);
      analogWrite(IN4_PIN, 200); //the speed value of motorB is val
    }
    if (Angle > 90) {
      f = (float)(180 - Angle) / 90;
      analogWrite(IN1_PIN, 200); //the speed value of motorA is val
      analogWrite(IN2_PIN, LOW);
      analogWrite(IN3_PIN, LOW );
      analogWrite(IN4_PIN, (float)(200 * f)); //the speed value of motorB is val
    }
  }
}
