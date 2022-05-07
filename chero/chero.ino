#include "header.h"
#include "utilities.h"
#include "printFunctions.h"
#include "measureVoltage.h"
#include "readSerial.h"
#include "parseCommand.h"

void setup() {
  Serial.begin(BAUD_RATE);

  pinMode(lowBeamsRelayPin, OUTPUT);
  digitalWrite(lowBeamsRelayPin, HIGH);
  pinMode(highBeamsRelayPin, OUTPUT);
  digitalWrite(highBeamsRelayPin, HIGH);
  pinMode(buttonLedRelayPin, OUTPUT);
  digitalWrite(buttonLedRelayPin, HIGH);

  pinMode(pushButtonPin, INPUT_PULLUP);

  systemInfo();
}

void loop() {
  if(digitalRead(pushButtonPin) == LOW && millis() - pushButtonTimestamp > pushButtonDelay) {
    pushButtonTimestamp = millis();
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

  char *buf = readSerial();
  if (buf != NULL) parseCommand(buf);
}
