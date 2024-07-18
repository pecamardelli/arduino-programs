#ifndef _POTENTIOMETER_H_
#define _POTENTIOMETER_H_

#include <Arduino.h>

class Potentiometer {
 private:
  int pin;
  int reading = 0;

 public:
  Potentiometer();
  Potentiometer(int);
  ~Potentiometer();
  int getMeasure();
  void setPin(int);
};

#endif