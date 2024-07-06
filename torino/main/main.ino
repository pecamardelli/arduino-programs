#include "header.h"

System sys;
SmallDisplay smallDisplay;
Clock clock;
Commander commander;
CurrentSensor currentSensor;
Flowmeter flowmeter;
TempSensor tempSensor;
VoltSensor voltSensor;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");

  Serial.println(sys.systemData.hostname);

  smallDisplay.begin();
  smallDisplay.drawTorinoLogo();
  clock.begin();
  tempSensor.begin();
}

void loop() {
  if (Serial.available()) {
    String input = commander.getInput();
    commander.exec(input);
  }

  // SMALL DISPLAY
  readings values = tempSensor.getReadings();

  smallDisplay.display(clock.getFullDate(), clock.getTime(),
                       String(round(values.temp)) + "c",
                       String(round(values.humidity)) + "%");

  // Print results to Serial Monitor to 2 decimal places
  volts = voltSensor.getMeasure();
  amperes = currentSensor.getMeasure();
  pulses = flowmeter.checkPulse();

  Serial.print("Current = ");
  Serial.println(amperes, 2);
}
