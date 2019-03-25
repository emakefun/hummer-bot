#include "IR_remote.h"

#define IN1_PIN 6   // PWMB
#define IN2_PIN 10  // DIRB  ---  right
#define IN4_PIN 9   // PWMA
#define IN3_PIN 5   // DIRA  ---  left

int RECV_PIN = 12;//Define the infrared receiver pin to 12

long expedite1 = 0xFFC23D;
long expedite2 = 0xFFB04F;
long advence = 0xFF02FD;
long back = 0xFF9867;
long stop = 0xFFA857;
long left = 0xFFE01F;
long right = 0xFF906F;
static int val = 0;
IRrecv irrecv(RECV_PIN);

decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Initialize the infrared receiver
}

void loop() {
  if (irrecv.decode( &results)) {
    if (results.value == advence) {
      analogWrite(IN1_PIN, 0);//the speed value of motorA is val
      analogWrite(IN2_PIN, val);
      analogWrite(IN3_PIN, val);
      analogWrite(IN4_PIN, 0); //the speed value of motorA is val
      delay(500);
      irrecv.resume(); // Receive the next value
    }
    if (results.value == expedite1) {
      val += 20;
      if (val >= 240)
      {
        val = 255;
      }
      irrecv.resume(); // Receive the next value
    }
    if (results.value == expedite2) {
      val -= 20;
      if (val <= 20)
      {
        val = 0;
      }
      irrecv.resume(); // Receive the next value
    }
    if (results.value == stop) {
      analogWrite(IN1_PIN, 0);
      analogWrite(IN2_PIN, 0);
      analogWrite(IN3_PIN, 0);
      analogWrite(IN4_PIN, 0);
      delay(500);
      irrecv.resume(); // Receive the next value
    }
    if (results.value == left) {
      analogWrite(IN1_PIN, val);
      analogWrite(IN2_PIN, 0); //the speed value of motorA is val
      analogWrite(IN3_PIN, val);
      analogWrite(IN4_PIN, 0); //the speed value of motorA is val
      delay(500);
      analogWrite(IN1_PIN, 0);
      analogWrite(IN2_PIN, 0); //the speed value of motorA is 0
      analogWrite(IN3_PIN, 0);
      analogWrite(IN4_PIN, 0); //the speed value of motorB is 0
      irrecv.resume(); // Receive the next value
    }
    if (results.value == right) {
      analogWrite(IN1_PIN, 0);//the speed value of motorA is val
      analogWrite(IN2_PIN, val);
      analogWrite(IN3_PIN, 0);//the speed value of motorA is val
      analogWrite(IN4_PIN, val);
      delay(500);
      analogWrite(IN1_PIN, 0);
      analogWrite(IN2_PIN, 0); //the speed value of motorA is 0
      analogWrite(IN3_PIN, 0);
      analogWrite(IN4_PIN, 0); //the speed value of motorB is 0
      irrecv.resume(); // Receive the next value
    }
    if (results.value == back) {
      analogWrite(IN1_PIN, val);
      analogWrite(IN2_PIN, 0); //the speed value of motorA is val
      analogWrite(IN3_PIN, 0); //the speed value of motorA is val
      analogWrite(IN4_PIN, val);
      delay(500);
      irrecv.resume(); // Receive the next value
    }
    Serial.println(results.value, HEX);//The hexadecimal line feed output code
    Serial.println();//For the convenience of viewing the output, add a blank line
    irrecv.resume(); // Receive the next value
  }
}
