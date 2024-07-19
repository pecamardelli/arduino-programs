#include "Potentiometer.h"

Potentiometer::Potentiometer() {}
Potentiometer::Potentiometer(int _pin) { pin = _pin; }

Potentiometer::~Potentiometer() {}

void Potentiometer::setPin(int _pin) { pin = _pin; }

int Potentiometer::getMeasure() {
  // reading = round(analogRead(pin) / 5) * 5;
  reading = analogRead(pin);
  if (reading % 2 == 0)
    return reading;
  else
    return reading + 1;
}