#include "header.h"

System sys;
SmallDisplay smallDisplay;
Clock clock;
Commander commander;
CurrentSensor currentSensor;
Flowmeter flowmeter;
TempSensor tempSensor;
VoltSensor voltSensor;
SDCard sdCard;
CoolantTempSensor coolantTempSensor;
TempGauge tempGauge;

void setup() {
  Serial.begin(9600);
  Serial.println(sys.systemData.hostname);

  smallDisplay.begin();
  smallDisplay.drawTorinoLogo();
  clock.begin();
  tempSensor.begin();
  sdCard.begin();
}

void loop() {
  if (Serial.available()) {
    String input = commander.getInput();
    commander.exec(input);
  }

  // SMALL DISPLAY
  readings values = tempSensor.getReadings();

  // smallDisplay.display(clock.getFullDate(), clock.getTime(),
  //                      String(round(values.temp)) + "c",
  //                      String(round(values.humidity)) + "%");

  // volts = voltSensor.getMeasure();
  // amperes = currentSensor.getMeasure();
  // coolantTemp = coolantTempSensor.getMeasure();
  // pulses = flowmeter.checkPulse();
  // stepper.forward();
  // Serial.println(potentiometer.getMeasure());
  tempGauge.loop();
}
