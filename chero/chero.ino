#include "header.h"
#include "measureVoltage.h"

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println(F("Relay Management for Jeep Grand Cherokee WJ 2005"));
  Serial.print(F("Version: "));
  Serial.println(VERSION);

  pinMode(lowBeamsRelayPin, OUTPUT);
  digitalWrite(lowBeamsRelayPin, HIGH);
  pinMode(highBeamsRelayPin, OUTPUT);
  digitalWrite(highBeamsRelayPin, HIGH);
  pinMode(buttonLedRelayPin, OUTPUT);
  digitalWrite(buttonLedRelayPin, HIGH);

  pinMode(pushButtonPin, INPUT_PULLUP);

  Serial.print(F("Low beams pin number: "));
  Serial.println(lowBeamsRelayPin);
  Serial.print(F("High beams pin number: "));
  Serial.println(highBeamsRelayPin);
  Serial.print(F("Button led pin number: "));
  Serial.println(buttonLedRelayPin);
  Serial.print(F("Push button pin number: "));
  Serial.println(pushButtonPin);
}

void loop() {
  if(digitalRead(pushButtonPin) == LOW && millis() - pushButtonTimestamp > pushButtonDelay) {
    pushButtonTimestamp = millis();
    auxiliaryBeamsLastState = auxiliaryBeamsEnabled;
    auxiliaryBeamsEnabled = !auxiliaryBeamsEnabled;
    Serial.println("Auxiliary Beams Enabled: " + String(auxiliaryBeamsEnabled));
  }

  if(auxiliaryBeamsEnabled) {
    lowBeamsSensorValue = measureVoltage(lowBeamsSensorPin);
    highBeamsSensorValue = measureVoltage(highBeamsSensorPin);

    digitalWrite(buttonLedRelayPin, LOW);

    if(highBeamsSensorValue >= auxiliaryBeamsVoltageThreshold) {
      digitalWrite(lowBeamsRelayPin, HIGH);
      digitalWrite(highBeamsRelayPin, LOW);
    }
    else if(lowBeamsSensorValue >= auxiliaryBeamsVoltageThreshold) {
      digitalWrite(lowBeamsRelayPin, LOW);
      digitalWrite(highBeamsRelayPin, HIGH);
    }
    else {
      digitalWrite(lowBeamsRelayPin, HIGH);
      digitalWrite(highBeamsRelayPin, HIGH);
    }
  }
  else {
    digitalWrite(lowBeamsRelayPin, HIGH);
    digitalWrite(highBeamsRelayPin, HIGH);
    digitalWrite(buttonLedRelayPin, HIGH);
  }
}
