#include "StepperMotor.h"

StepperMotor::StepperMotor(/* args */) { myStepper.setSpeed(1); }

StepperMotor::~StepperMotor() {}

void StepperMotor::forward() { myStepper.step(1); }
void StepperMotor::backward() { myStepper.step(-1); }
void StepperMotor::stop() { myStepper.step(0); }
