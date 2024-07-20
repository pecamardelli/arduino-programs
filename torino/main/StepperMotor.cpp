#include "StepperMotor.h"

StepperMotor::StepperMotor(/* args */) { myStepper.setSpeed(1); }

StepperMotor::~StepperMotor() {}

void StepperMotor::forward(int steps) { myStepper.step(steps); }
void StepperMotor::backward(int steps) { myStepper.step(-steps); }

void StepperMotor::stop() {
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
}
