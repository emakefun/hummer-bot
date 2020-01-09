#define IN1_PIN 6   // PWMB
#define IN2_PIN 10  // DIRB  ---  right
#define IN4_PIN 9   // PWMA
#define IN3_PIN 5   // DIRA  ---  left
const int leftPin = A3;
const int rightPin = A4;
float LeftValue,RightValue;
int Angle;
float f;
void setup()
{
  Serial.begin(9600);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  delay(1000);
}

void loop()
{
  LeftValue = analogRead(leftPin) / 10;
  RightValue = analogRead(rightPin) / 10;
  if ( (LeftValue > 50) && (RightValue > 50)) {
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, HIGH);
  } else {
    if (LeftValue > RightValue) {
      Angle = ((float)(RightValue/LeftValue)) * 90;
    } else if (LeftValue < RightValue) {
      Angle = (90 - ((float)(LeftValue/RightValue)) * 90) + 90;
    }
    Serial.println(Angle);
    if (Angle <= 90) {
      f = (float)(Angle) / 90;
      analogWrite(IN1_PIN, (float)(120 * f)); //the speed value of motorA is val
      analogWrite(IN2_PIN, LOW);
      analogWrite(IN3_PIN, LOW);
      analogWrite(IN4_PIN, 120); //the speed value of motorB is val
    }
    if (Angle > 90) {
      f = (float)(180 - Angle) / 90;
      analogWrite(IN1_PIN, 120); //the speed value of motorA is val
      analogWrite(IN2_PIN, LOW);
      analogWrite(IN3_PIN, LOW );
      analogWrite(IN4_PIN, (float)(120 * f)); //the speed value of motorB is val
    }
  }
}
