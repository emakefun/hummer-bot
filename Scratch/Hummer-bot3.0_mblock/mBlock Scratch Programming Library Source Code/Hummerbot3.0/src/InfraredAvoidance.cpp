#include "InfraredAvoidance.h"

InfraredAvoidance::InfraredAvoidance(byte pin1, byte pin2, byte pin3, byte pin4)
{
  LeftInfraredAvoidancePin = pin1;
  RightInfraredAvoidancePin = pin2;
  LeftPhotoresistorPin = pin3;
  RightPhotoresistorPin = pin4;
  pinMode(LeftInfraredAvoidancePin, INPUT);
  pinMode(RightInfraredAvoidancePin, INPUT);
  pinMode(LeftPhotoresistorPin, INPUT);
  pinMode(RightPhotoresistorPin, INPUT);
}
InfraredAvoidance::InfraredAvoidance(byte pin1, byte pin2)
{
  LeftInfraredAvoidancePin = pin1;
  RightInfraredAvoidancePin = pin2;
  pinMode(LeftInfraredAvoidancePin, INPUT);
  pinMode(RightInfraredAvoidancePin, INPUT);
}

void InfraredAvoidance::PhotosensitivePin(byte pin1, byte pin2)
{
  LeftPhotoresistorPin = pin1;
  RightPhotoresistorPin = pin2;
  pinMode(LeftPhotoresistorPin, INPUT);
  pinMode(RightPhotoresistorPin, INPUT);
}
uint8_t InfraredAvoidance ::GetLeftInfraredAvoidanceValue(void)
{
  return digitalRead(LeftInfraredAvoidancePin);
}

uint8_t InfraredAvoidance ::GetRightInfraredAvoidanceValue(void)
{
  return digitalRead(RightInfraredAvoidancePin);
}

uint8_t InfraredAvoidance ::GetInfraredAvoidanceValue(void)
{
    ItPins[0] = RightInfraredAvoidancePin;
    ItPins[1] = LeftInfraredAvoidancePin;
    dat = 0;
    dat = digitalRead(ItPins[0]);
    dat |= digitalRead(ItPins[1]) << 1;
    return dat;
}

int InfraredAvoidance ::GetLeftPhotoresistorValue(void)
{
    return analogRead(LeftPhotoresistorPin);
}

int InfraredAvoidance ::GetRightPhotoresistorValue(void)
{
    return analogRead(RightPhotoresistorPin);
}
