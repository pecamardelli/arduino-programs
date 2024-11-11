#ifndef _TEMP_GAUGE_H_
#define _TEMP_GAUGE_H_

/*
  20 -> 390° -> 0 steps
  30 -> 396° -> 0 steps
  40° -> 412° -> 50 steps
  50° -> 428° -> 78 steps
  60° -> 442° -> 118 steps
  70° -> 464° -> 158 steps
  80° -> 482° -> 200 steps
  90° -> 512° -> 244 steps
  100° -> 530° -> 286 steps
  110° -> 554° -> 325 steps
  120° -> 564° -> 335 steps
  130° -> 576° -> 370 steps
  140° -> 596° -> 410 steps
*/

#include <Stepper.h>

#include "Potentiometer.h"
// #include "StepperMotor.h"
#include "common.h"

extern int debugMode;

typedef struct Conversion {
  uint8_t temp;
  uint16_t angle;
  uint16_t steps;
};

class TempGauge {
 private:
  const Conversion conversions[13] = {
      {20, 390, 0},    {30, 396, 50},   {40, 400, 50},   {50, 428, 78},
      {60, 440, 118},  {70, 463, 158},  {80, 482, 200},  {90, 509, 244},
      {100, 531, 286}, {110, 549, 325}, {120, 560, 335}, {130, 573, 370},
      {140, 586, 410}};
  const uint8_t minTemp = 20;
  const uint8_t maxTemp = 140;
  const uint16_t minAngle = 390;
  const uint16_t maxAngle = 590;
  uint16_t steps = 0;
  float currentTemperature = minTemp;
  float lastTemperature = currentTemperature;
  Potentiometer angleSensor = Potentiometer(8);
  unsigned long lastStepMillis = 0;
  unsigned long millisBetweenSteps = 400;
  // Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
  Stepper stepper = Stepper(2038, 8, 10, 9, 11);
  bool inited = false;
  void setup();
  int tempToAngle(float);
  unsigned long getMillisBetweenSteps(int, int);

 public:
  TempGauge(/* args */);
  ~TempGauge();
  void setTemperature(float);
  void loop();
  EXEC_STATUSES exec(String[]);
};

#endif