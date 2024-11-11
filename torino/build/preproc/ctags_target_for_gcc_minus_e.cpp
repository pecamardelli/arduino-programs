# 1 "/home/pablin/arduino-programs/torino/main/main.ino"
# 1 "/home/pablin/arduino-programs/torino/main/main.ino"
# 2 "/home/pablin/arduino-programs/torino/main/main.ino" 2

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
// CircularDisplay circularDisplay;

void setup() {
  Serial.begin(9600);
  Serial.println(sys.systemData.hostname);

  // Relay that commands direct power input.
  pinMode(48, 0x1);
  digitalWrite(48, 0x0);

  smallDisplay.begin();
  smallDisplay.drawTorinoLogo();
  clock.begin();
  tempSensor.begin();
  // sdCard.begin();
  // circularDisplay.setup();
}

void loop() {
  if (Serial.available()) {
    String input = commander.getInput();
    commander.exec(input);
  }

  readings values = tempSensor.getReadings();

  // SMALL DISPLAY
  // smallDisplay.display(clock.getFullDate(), clock.getTime(),
  //                      String(round(values.temp)) + "c",
  //                      String(round(values.humidity)) + "%");

  volts = voltSensor.getMeasure();
  ignition = volts > 10;
  amperes = currentSensor.getMeasure();
  coolantTemp = coolantTempSensor.getMeasure();
  tempGauge.setTemperature(coolantTemp);
  tempGauge.loop();
  _pulses = flowmeter.checkPulse();
  // circularDisplay.loop();

  if (debugMode == READINGS) {
    Serial.print("Volts: ");
    Serial.print(volts);
    Serial.print(" - Amp: ");
    Serial.print(amperes);
    Serial.print(" - Temp: ");
    Serial.print(coolantTemp);
    Serial.print(" - Pulses: ");
    Serial.print(_pulses);
    Serial.print(" - Ignition: ");
    Serial.println(ignition);
  }
}
