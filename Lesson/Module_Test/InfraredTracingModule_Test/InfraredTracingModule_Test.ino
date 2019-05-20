void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
}
void loop() {
  int left, center, right;
  left = digitalRead(A0);
  center = digitalRead(A1);
  right = digitalRead(A2);
  Serial.print("right:");
  Serial.print(right);
  Serial.print("   ");
  Serial.print("center:");
  Serial.print(center);
  Serial.print("   ");
  Serial.print("left:");
  Serial.print(left);
  Serial.println("   ");
}
