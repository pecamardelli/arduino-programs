void checkButton() {
  if(digitalRead(PUSH_BUTTON) == LOW && millis() - pushButtonTimestamp > PUSH_BUTTON_DELAY) {
    pushButtonTimestamp = millis();
    auxiliaryBeamsEnabled = !auxiliaryBeamsEnabled;
    Serial.println("Enabled: " + String(auxiliaryBeamsEnabled));
  }
}
