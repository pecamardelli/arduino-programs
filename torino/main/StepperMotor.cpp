#include "StepperMotor.h"

StepperMotor::StepperMotor(/* args */) { myStepper.setSpeed(20); }

StepperMotor::~StepperMotor() {}

void StepperMotor::engage() { myStepper.step(4); }
