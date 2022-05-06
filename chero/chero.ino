#include "header.h"

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
  pinMode(TEST_BUTTON1, INPUT_PULLUP);
  pinMode(TEST_BUTTON2, INPUT_PULLUP);

  Serial.print(F("Relay 1 pin (low beams): "));
  Serial.println(RELAY1);
  Serial.print(F("Relay 2 pin (high beams): "));
  Serial.println(RELAY2);
  Serial.print(F("Relay 3 pin (button led): "));
  Serial.println(RELAY3);
  Serial.print(F("Relay 4 pin (unused): "));
  Serial.println(RELAY4);
  Serial.print(F("Push button pin: "));
  Serial.println(PUSH_BUTTON);
}

void loop() {
  if(digitalRead(PUSH_BUTTON) == LOW && millis() - pushButtonTimestamp > PUSH_BUTTON_DELAY) {
    pushButtonTimestamp = millis();
    auxiliaryBeamsLastState = auxiliaryBeamsEnabled;
    auxiliaryBeamsEnabled = 1 - auxiliaryBeamsEnabled;
    Serial.println("Auxiliary Beams Enabled: " + String(auxiliaryBeamsEnabled));
  }

  if(auxiliaryBeamsEnabled == LOW) {
    lowBeams = digitalRead(TEST_BUTTON1);
    digitalWrite(RELAY1, lowBeams);
    
    if(lowBeams == HIGH) digitalWrite(RELAY2, digitalRead(TEST_BUTTON2));
    else digitalWrite(RELAY2, HIGH);
  }
  else {
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY2, HIGH);
  }

  if(auxiliaryBeamsLastState != auxiliaryBeamsEnabled) {
    digitalWrite(RELAY3, auxiliaryBeamsEnabled);
  }
}
