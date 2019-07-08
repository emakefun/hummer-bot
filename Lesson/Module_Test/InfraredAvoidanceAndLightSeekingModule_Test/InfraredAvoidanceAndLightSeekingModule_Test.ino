const int LeftAvoidancePin = 12;
const int RightAvoidancePin = A5;
const int LeftLightPin = A3;
const int RightLightPin = A4;

int dl, dr, LL, LR;
void setup() {
  Serial.begin(9600);
  pinMode(LeftAvoidancePin, INPUT);
  pinMode(RightAvoidancePin, INPUT);
  pinMode(LeftLightPin, INPUT);
  pinMode(RightLightPin, INPUT);
  delay(1000);
}
void loop() {
  dl = digitalRead(LeftAvoidancePin);
  dr = digitalRead(RightAvoidancePin);
  LL = analogRead(LeftLightPin);
  LR = analogRead(RightLightPin);
  Serial.print("LeftAvoidance:");
  Serial.print(dl);
  Serial.print("   ");
  Serial.print("RightAvoidance:");
  Serial.println(dr);
  Serial.print("LeftLight:");
  Serial.print(LL);
  Serial.print("   ");
  Serial.print("RightLight:");
  Serial.println(LR);
  delay(1000);
}
