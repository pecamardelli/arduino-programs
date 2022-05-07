float measureVoltage(const int pin) {
  float vOUT  = 0.0;
  float vIN   = 0.0;
  float R1    = 30000.0;
  float R2    = 7500.0;
  int   value = 0;

  value = analogRead(pin);
  vOUT = (value * 5.0) / 1024.0;
  vIN = vOUT / (R2/(R1+R2));
  
  return vIN;
}
