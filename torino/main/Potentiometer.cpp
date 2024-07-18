#include "Potentiometer.h"

Potentiometer::Potentiometer() {}
Potentiometer::Potentiometer(int _pin) { pin = _pin; }

Potentiometer::~Potentiometer() {}

void Potentiometer::setPin(int _pin) { pin = _pin; }

int Potentiometer::getMeasure() {
  reading = analogRead(pin);
  return reading;
}