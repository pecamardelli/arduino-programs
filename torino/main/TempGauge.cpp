#include "TempGauge.h"

TempGauge::TempGauge(/* args */) {
  Serial.println(F("Temperature Gauge Initiating..."));
}

TempGauge::~TempGauge() {}

void TempGauge::setup() {
  int angle = angleSensor.getMeasure();

  if (angle < minAngle) {
    stepper.forward(1);
  } else if (angle > minAngle) {
    stepper.backward(1);
  } else {
    Serial.println(F("Temperature Gauge Inited."));
    stepper.stop();
    inited = true;
  }
}

void TempGauge::loop() {
  if (!inited) setup();

  if (currentTemperature != lastTemperature) {
    int currentAngle = angleSensor.getMeasure();
    int nextAngle = tempToAngle(currentTemperature);

    // Serial.print(currentAngle);
    // Serial.print(" ");
    // Serial.print(nextAngle);
    // Serial.print(" ");
    // Serial.println(conversions[0].angle);

    if (currentAngle < nextAngle) {
      stepper.forward(1);
      steps++;
    } else if (currentAngle > nextAngle) {
      stepper.backward(1);
      steps--;
    } else {
      stepper.stop();
      lastTemperature = currentTemperature;
    }
  }
}

void TempGauge::setTemperature(float temp) {
  if (temp < minTemp)
    currentTemperature = minTemp;
  else if (temp > maxTemp)
    currentTemperature = maxTemp;
  else
    currentTemperature = temp;
}

int TempGauge::tempToAngle(float temp) {
  int lowerIndex = temp / 10 - 2;
  int upperIndex = lowerIndex + 1;
  int angleDifference =
      conversions[upperIndex].angle - conversions[lowerIndex].angle;
  int tempDifference =
      conversions[upperIndex].temp - conversions[lowerIndex].temp;

  return conversions[lowerIndex].angle + (temp - conversions[lowerIndex].temp) /
                                             tempDifference * angleDifference;
}

EXEC_STATUSES TempGauge::exec(String args[]) {
  if (args[0].equals("temp")) {
    currentTemperature = args[1].toFloat();
    return COMMAND_SUCCESSFUL;
  }

  return NO_COMMAND;
}
