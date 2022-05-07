void systemInfo() {
  Serial.println(F("================================="));
  Serial.println(F(" Arduino Relay Management System"));
  Serial.println(F("---------------------------------"));
  Serial.print(F(" Software Version\t"));
  Serial.println(VERSION);
  Serial.print(F(" Board Type\t\t"));
  Serial.println(BOARD);
  Serial.print(F(" Serial Baud Rate\t"));
  Serial.println(BAUD_RATE);
  Serial.println(F("================================="));  
}

void pinInfo() {
  Serial.print(F("\nLow beams pin number:\t"));
  Serial.println(lowBeamsRelayPin);
  Serial.print(F("High beams pin number:\t"));
  Serial.println(highBeamsRelayPin);
  Serial.print(F("Button led pin number:\t"));
  Serial.println(buttonLedRelayPin);
  Serial.print(F("Push button pin number:\t"));
  Serial.println(pushButtonPin);
}

void showCommands() {
  Serial.println(F("\nhelp\t\tDisplay available commands."));
  Serial.println(F("sysinfo\t\tDisplay system information."));
  Serial.println(F("show [arg]\tDisplay information of the given parameter."));
}

void showHelp() {
  Serial.println(F("\nshow help\tDisplay available arguments for 'show' command."));
  Serial.println(F("show sysinfo\tDisplay system information."));
  Serial.println(F("show pins\tDisplay pin information."));
}
