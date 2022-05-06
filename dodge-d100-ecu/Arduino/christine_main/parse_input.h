void parse_input(String input)
{
  reset_message();
  if(input.substring(0,4) == "help")
  {
    Serial.println(F("------------------------------------"));
    Serial.println(F("print [text]\t\t\tImprime el texto en pantalla."));
    Serial.println(F("dhtinfo\t\t\tMuestra informacion del sensor de temperatura."));
    Serial.println(F("temp\t\t\t\tDevuelve la temperatura actual."));
    Serial.println(F("humdt\t\t\t\tDevuelve la temperatura actual."));
    Serial.println(F("------------------------------------"));
  }
  else if(input.substring(0,5) == "print" && input.length() > 6)
  {
    char  buf[input.length() - 5];
    input.substring(6, input.length()).toCharArray(buf, input.length() - 5);
    printing_msg  = true;
    draw_string(buf,NOONE,NOONE);
  }
  else if(input.substring(0,8) == "resetmsg")
  {
    printing_msg  = false;
    reset_message();
  }
  else if(input.substring(0,8) == "dhtinfo")
  {
    // Print temperature sensor details.
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println(F("Temperature Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
    Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
    Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
    Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
    Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
    Serial.println("");
    // Print humidity sensor details.
    dht.humidity().getSensor(&sensor);
    Serial.println(F("Humidity Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
    Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
    Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
    Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
    Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  }
  else if(input.substring(0,4) == "temp")
  {
    int temp  = get_temp();

    if(temp == NOONE)
    {
      Serial.print(F("ERROR"));
    }
    else
    {
      Serial.print(temp);
    }
  }
  else if(input.substring(0,5) == "humdt")
  {
    int humdt = get_humdt();

    if(humdt == NOONE)
    {
      Serial.print(F("ERROR"));
    }
    else
    {
      Serial.print(humdt);
    }
  }
}
