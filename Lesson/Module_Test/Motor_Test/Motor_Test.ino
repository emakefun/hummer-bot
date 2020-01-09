#define IN1_PIN 6
#define IN2_PIN 10
#define IN3_PIN 5
#define IN4_PIN 9

void setup() {
  Serial.begin(9600);
  pinMode(IN1_PIN, OUTPUT);
  digitalWrite(IN1_PIN, LOW); // When not sending PWM, we want it low
  pinMode(IN2_PIN, OUTPUT);
  digitalWrite(IN2_PIN, LOW); // When not sending PWM, we want it low
  pinMode(IN3_PIN, OUTPUT);
  digitalWrite(IN3_PIN, LOW); // When not sending PWM, we want it low
  pinMode(IN4_PIN, OUTPUT);
  digitalWrite(IN4_PIN, LOW); // When not sending PWM, we want it low
}

void loop() {
  analogWrite(IN1_PIN, 100);
  analogWrite(IN2_PIN, LOW);
  analogWrite(IN3_PIN, LOW);
  analogWrite(IN4_PIN, 100);
  delay(5000);
  //******** ******************************//forward
  analogWrite(IN1_PIN, HIGH);
  analogWrite(IN2_PIN, HIGH);
  analogWrite(IN3_PIN, HIGH);
  analogWrite(IN4_PIN, HIGH);
  delay(1000);//********************************************//stop
  analogWrite(IN1_PIN, LOW);
  analogWrite(IN2_PIN, 100);
  analogWrite(IN3_PIN, 100);
  analogWrite(IN4_PIN, LOW);
  delay(5000);//*********************************************//back
  analogWrite(IN1_PIN, HIGH);
  analogWrite(IN2_PIN, HIGH);
  analogWrite(IN3_PIN, HIGH);
  analogWrite(IN4_PIN, HIGH);
  delay(1000);
  //******* ***************************************//stop
  analogWrite(IN1_PIN, 200);
  analogWrite(IN2_PIN, LOW);
  analogWrite(IN3_PIN, 200);
  analogWrite(IN4_PIN, LOW);
  delay(3000);
  //*******************************************//left
  analogWrite(IN1_PIN, HIGH);
  analogWrite(IN2_PIN, HIGH);
  analogWrite(IN3_PIN, HIGH);
  analogWrite(IN4_PIN, HIGH);
  delay(1000); //*******************************************//stop
  analogWrite(IN1_PIN, LOW);
  analogWrite(IN2_PIN, 200);
  analogWrite(IN3_PIN, LOW);
  analogWrite(IN4_PIN, 200);
  delay(3000);//*** ***************************************//right
}
