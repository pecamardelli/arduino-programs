#ifndef _TEMP_GAUGE_H_
#define _TEMP_GAUGE_H_

/*
  30° -> 395
  40° -> 412
  50° -> 428
  60° -> 442
  70° -> 464
  80° -> 482
  90° -> 512
  100° -> 530
  110° -> 554
  120° -> 564
  130° -> 576
  140° -> 596
*/

#include "Potentiometer.h"
#include "StepperMotor.h"
#include "common.h"

class TempGauge {
 private:
  const uint8_t minTemp = 30;
  const uint8_t maxTemp = 140;
  const uint16_t minAngle = 390;
  const uint16_t maxAngle = 590;
  float temperature = 390;
  Potentiometer angleSensor = Potentiometer(0);
  StepperMotor stepper;
  uint8_t parseTemperature(float);

 public:
  TempGauge(/* args */);
  ~TempGauge();
  void setTemperature(float);
  EXEC_STATUSES exec(String[]);
};

#endif