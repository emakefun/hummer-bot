#define IN1_PIN 6   // PWMB
#define IN2_PIN 10  // DIRB  ---  right
#define IN4_PIN 9   // PWMA
#define IN3_PIN 5   // DIRA  ---  left
const int leftPin = A2;
const int rightPin = A4;
float LeftValue,RightValue;
int Angle;
float f;
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
    analogWrite(IN1_PIN, 0); //the speed value of motorA is val
    analogWrite(IN2_PIN, 0);
    analogWrite(IN3_PIN, 0);
    analogWrite(IN4_PIN, 0); //the speed value of motorB is val
  } else {
    if (LeftValue > RightValue) {
      Angle = ((float)(RightValue/LeftValue)) * 90;
    } else if (LeftValue < RightValue) {
      Angle = (90 - ((float)(LeftValue/RightValue)) * 90) + 90;
    }
    Serial.println(Angle);
    if (Angle <= 90) {
      f = (float)(Angle) / 90;
      analogWrite(IN1_PIN, 0); //the speed value of motorA is val
      analogWrite(IN2_PIN, 200);
      analogWrite(IN3_PIN, (float)(200 * f));
      analogWrite(IN4_PIN, 0); //the speed value of motorB is val
    }
    if (Angle > 90) {
      f = (float)(180 - Angle) / 90;
      analogWrite(IN1_PIN, 0); //the speed value of motorA is val
      analogWrite(IN2_PIN, (float)(200 * f));
      analogWrite(IN3_PIN, 200 );
      analogWrite(IN4_PIN, 0); //the speed value of motorB is val
    }
  }
}
