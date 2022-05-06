#include "header.h"
#include "checkButton.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  Serial.println(F("Relay Management for Jeep Grand Cherokee WJ 2005"));
  Serial.print(F("Version: "));
  Serial.println(VERSION);

  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1, HIGH);
  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY2, HIGH);
  pinMode(RELAY3, OUTPUT);
  digitalWrite(RELAY3, HIGH);
  pinMode(RELAY4, OUTPUT);
  digitalWrite(RELAY4, HIGH);

  pinMode(PUSH_BUTTON, INPUT_PULLUP);

  Serial.print(F("Relay 1 pin: "));
  Serial.println(RELAY1);
  Serial.print(F("Relay 2 pin: "));
  Serial.println(RELAY2);
  Serial.print(F("Relay 3 pin: "));
  Serial.println(RELAY3);
  Serial.print(F("Relay 4 pin: "));
  Serial.println(RELAY4);
  Serial.print(F("Push button pin: "));
  Serial.println(PUSH_BUTTON);
  
}

void loop() {
  if(digitalRead(PUSH_BUTTON) == LOW && millis() - pushButtonTimestamp > PUSH_BUTTON_DELAY) {
    pushButtonTimestamp = millis();
    auxiliaryBeamsEnabled = !auxiliaryBeamsEnabled;
    Serial.println("Auxiliary Beams Enabled: " + String(auxiliaryBeamsEnabled));
  }
}
