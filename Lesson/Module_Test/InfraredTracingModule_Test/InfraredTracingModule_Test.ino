void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
}
void loop() {
  int left, centre, right;
  left = digitalRead(A0);
  centre = digitalRead(A1);
  right = digitalRead(A2);
  Serial.print("right:");
  Serial.print(right);
  Serial.print("   ");
  Serial.print("centre:");
  Serial.print(centre);
  Serial.print("   ");
  Serial.print("left:");
  Serial.print(left);
  Serial.println("   ");
}
