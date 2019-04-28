#include "InfraredTracing.h"

byte dat = 0;
byte ItPins[3];

InfraredTracing::InfraredTracing(byte pin1, byte pin2, byte pin3)
{
    InfraredTracingPin1 = pin1;
    InfraredTracingPin2 = pin2;
    InfraredTracingPin3 = pin3;
    value = 0;
    InfraredTracingMode = E_INFRARED_SENSOR_3;
    begin();
}

void InfraredTracing::begin()
{
  pinMode(InfraredTracingPin1, INPUT);
  pinMode(InfraredTracingPin2, INPUT);
  pinMode(InfraredTracingPin3, INPUT);
  ItPins[0] = InfraredTracingPin1;
  ItPins[1] = InfraredTracingPin2;
  ItPins[2] = InfraredTracingPin3;
}

byte InfraredTracing::GetValue()
{
    dat = 0;
    dat = digitalRead(ItPins[0]);
    dat |= digitalRead(ItPins[1]) << 1;
    dat |= digitalRead(ItPins[2]) << 2;
    return dat;
}
byte InfraredTracing::GetLeftValue()
{
  return digitalRead(InfraredTracingPin1);
}

byte InfraredTracing::GetMiddleValue()
{
  return digitalRead(InfraredTracingPin2);
}

byte InfraredTracing::GetRightValue()
{
  return digitalRead(InfraredTracingPin3);
}
