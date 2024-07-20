// https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/

#ifndef _STEPPER_MOTOR_H_
#define _STEPPER_MOTOR_H_

#include <Arduino.h>
#include <Stepper.h>

class StepperMotor {
 private:
  // Defines the number of steps per rotation
  const int stepsPerRevolution = 2048;
  // Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
  Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

 public:
  StepperMotor(/* args */);
  ~StepperMotor();

  void forward(int);
  void backward(int);
  void stop();
};

#endif