#ifndef _TEMP_GAUGE_H_
#define _TEMP_GAUGE_H_

#include "Potentiometer.h"
#include "StepperMotor.h"

class TempGauge {
 private:
  float temperature = 0;
  StepperMotor stepper;
  Potentiometer angleSensor = Potentiometer(0);

 public:
  TempGauge(/* args */);
  ~TempGauge();
  void setTemperature(float);
};

#endif