#include "TempGauge.h"

TempGauge::TempGauge(/* args */) {}

TempGauge::~TempGauge() {}

void TempGauge::setTemperature(float currentTemperature) {
  uint8_t parsedTemperature = parseTemperature(currentTemperature);
  int angle = angleSensor.getMeasure();

  Serial.print(angle);
  Serial.print(" ");
  Serial.println(temperature);

  if (angle < temperature || angle < minAngle) {
    stepper.forward();
  } else if (angle > temperature || angle > maxAngle) {
    stepper.backward();
  } else {
    stepper.stop();
  }
}

uint8_t TempGauge::parseTemperature(float currentTemperature) {
  if (currentTemperature < minTemp) return minTemp;
  if (currentTemperature > maxTemp) return maxTemp;
  return (uint8_t)currentTemperature;
}

EXEC_STATUSES TempGauge::exec(String args[]) {
  if (args[0].equals("temp")) {
    temperature = args[1].toFloat();
    return COMMAND_SUCCESSFUL;
  }
  return NO_COMMAND;
}
