#include "CoolantTempSensor.h"

OneWire ourWire(2);
DallasTemperature sensors(&ourWire);

CoolantTempSensor::CoolantTempSensor(/* args */) { sensors.begin(); }

CoolantTempSensor::~CoolantTempSensor() {}

float CoolantTempSensor::getMeasure() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}