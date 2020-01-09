#define IN1_PIN 6   // PWMB
#define IN2_PIN 10  // DIRB  ---  right
#define IN4_PIN 9   // PWMA
#define IN3_PIN 5   // DIRA  ---  left
const int leftPin = 12;
const int rightPin = A5;
byte LeftValue, RightValue;
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
  LeftValue = digitalRead(leftPin);
  RightValue = digitalRead(rightPin);
  if (LeftValue >= 1 && RightValue >= 1)
  {
    analogWrite(IN1_PIN, 100); //the speed value of motorA is val
    analogWrite(IN2_PIN, LOW);
    analogWrite(IN3_PIN, LOW);
    analogWrite(IN4_PIN, 100); //the speed value of motorB is val
    Serial.print(LeftValue);
    Serial.print("   ");
    Serial.print(RightValue);
    Serial.print("   ");
    Serial.println("go");//********************************************//forward
  } else if (LeftValue >= 1 && RightValue < 1) {
    analogWrite(IN1_PIN, 200);//the speed value of motorA is 200
    analogWrite(IN2_PIN, 0);
    analogWrite(IN3_PIN, 200);//the speed value of motorB is 200
    analogWrite(IN4_PIN, 0);
    Serial.print(LeftValue);
    Serial.print("   ");
    Serial.print(RightValue);
    Serial.print("   ");
    Serial.println("Turning left");
    delay(300);
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, HIGH);
    delay(1000); //********************************************//Turning left
  } else if (LeftValue < 1 && RightValue < 1) {
    analogWrite(IN1_PIN, 0);
    analogWrite(IN2_PIN, 255); //the speed value of motorA is 255
    analogWrite(IN3_PIN, 0);
    analogWrite(IN4_PIN, 255); //the speed value of motorB is 255
    Serial.print(LeftValue);
    Serial.print("   ");
    Serial.print(RightValue);
    Serial.print("   ");
    Serial.println("Turning around");
    delay(500);
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, HIGH);
    delay(1000); //********************************************//Turning around
  } if (LeftValue < 1 && RightValue >= 1) {
    analogWrite(IN1_PIN, 0);
    analogWrite(IN2_PIN, 200);//the speed value of motorA is 200
    analogWrite(IN3_PIN, 0);
    analogWrite(IN4_PIN, 200);//the speed value of motorB is 200
    Serial.print(LeftValue);
    Serial.print("   ");
    Serial.print(RightValue);
    Serial.print("   ");
    Serial.println("Turning right");
    delay(300);
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, HIGH);
    delay(1000); //********************************************//Turning right
  }
}
