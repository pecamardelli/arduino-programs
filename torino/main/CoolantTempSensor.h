#ifndef _WATER_TEMP_SENSOR_H_
#define _WATER_TEMP_SENSOR_H_

#include <DallasTemperature.h>
#include <OneWire.h>

class CoolantTempSensor {
 private:
  /* data */
 public:
  CoolantTempSensor(/* args */);
  ~CoolantTempSensor();
  float getMeasure();
};

#endif