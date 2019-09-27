#include "IRremote.h"

#define IN1_PIN 6   // PWMB
#define IN2_PIN 10  // DIRB  ---  right
#define IN4_PIN 9   // PWMA
#define IN3_PIN 5   // DIRA  ---  left

int RECV_PIN = 11;//Define the infrared receiver pin to 12

long expedite1 = 0x43;
long expedite2 = 0x0d;
long up = 0x40;
long down = 0x19;
long stop = 0x15;
long left = 0x07;
long right = 0x09;
static int val = 160;
IRremote irrecv(RECV_PIN);


void setup() {
  Serial.begin(9600);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  irrecv.begin();
}

void loop() {
  byte irKeyCode;
  if (irKeyCode = irrecv.getCode())
  {
    if (irKeyCode == up) {
      analogWrite(IN1_PIN, val);//the speed value of motorA is val
      analogWrite(IN2_PIN, LOW);
      analogWrite(IN3_PIN, LOW);
      analogWrite(IN4_PIN, val); //the speed value of motorA is val
    }
    else if (irKeyCode == expedite1) {
      val += 20;
      if (val >= 240)
      {
        val = 255;
      }
    }
    else if (irKeyCode == expedite2) {
      val -= 20;
      if (val <= 20)
      {
        val = 0;
      }
    }
    else if (irKeyCode == stop) {
      digitalWrite(IN1_PIN, HIGH);
      digitalWrite(IN2_PIN, HIGH);
      digitalWrite(IN3_PIN, HIGH);
      digitalWrite(IN4_PIN, HIGH);
    }
    else if (irKeyCode == left) {
      analogWrite(IN1_PIN, val);
      analogWrite(IN2_PIN, LOW); //the speed value of motorA is val
      analogWrite(IN3_PIN, val);
      analogWrite(IN4_PIN, LOW); //the speed value of motorA is val
    }
    else if (irKeyCode == right) {
      analogWrite(IN1_PIN, LOW);//the speed value of motorA is val
      analogWrite(IN2_PIN, val);
      analogWrite(IN3_PIN, LOW);//the speed value of motorA is val
      analogWrite(IN4_PIN, val);
    }
    else if (irKeyCode == down) {
      analogWrite(IN1_PIN, LOW);
      analogWrite(IN2_PIN, val); //the speed value of motorA is val
      analogWrite(IN3_PIN, val); //the speed value of motorA is val
      analogWrite(IN4_PIN, LOW);
    }
  } else {
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, HIGH);
  }
}
