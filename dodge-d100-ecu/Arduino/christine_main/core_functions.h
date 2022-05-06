int get_temp()
{
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    //Serial.println(F("Error reading temperature!"));
    return NOONE;
  }
  else
  {
    //Serial.print(F("Temperature: "));
    //Serial.print(event.temperature);
    //Serial.println(F("°C"));
    return event.temperature;
  }
}

int get_humdt()
{
  sensors_event_t event;
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    //Serial.println(F("Error reading humidity!"));
    return NOONE;
  }
  else {
    //Serial.print(F("Humidity: "));
    //Serial.print(event.relative_humidity);
    //Serial.println(F("%"));
    return event.relative_humidity;
  }
}

void main_screen()
{
  my_lcd.Print("Hola Mundo!",CENTER,my_lcd.Get_Display_Height()/2);
  //draw_date_time();
  //draw_temp_icons();
  //draw_temp();  
}

void clk_config()
{
  if(digitalRead(BUTTON1) == LOW)
  {
    aux++;
    delay(50);
  }
  
  switch(screen)
  {
    case SCREEN_CLK_DAY:
      draw_string("CURRENT DAY", NOONE, NOONE);
      if(aux != last_aux)
      {
        if(aux < 1 || aux > 31)
        {
          aux = 1;
        }
        
        draw_big_numbers(aux, NOONE, NOONE, true);
        last_aux  = aux;
        dt.aux_day   = aux;
      }
      break;
    case SCREEN_CLK_MON:
      draw_string("CURRENT MONTH", NOONE, NOONE);
      if(aux != last_aux)
      {
        if(aux < 1 || aux > 12)
        {
          aux = 1;
        }
        
        draw_big_numbers(aux, NOONE, NOONE, true);
        last_aux  = aux;
        dt.aux_mon   = aux;
      }
      break;
    case SCREEN_CLK_YEAR:
      draw_string("CURRENT YEAR", NOONE, NOONE);
      if(aux != last_aux)
      {
        if(aux < 0 || aux > 99)
        {
          aux = 0;
        }
        
        draw_big_numbers(aux + 2000, NOONE, NOONE, true);
        last_aux  = aux;
        dt.aux_year  = aux;
      }
      break;
    case SCREEN_CLK_HOUR:
      draw_string("CURRENT HOUR", NOONE, NOONE);
      if(aux != last_aux)
      {
        if(aux < 0 || aux > 23)
        {
          aux = 0;
        }
        
        draw_big_numbers(aux, NOONE, NOONE, true);
        last_aux  = aux;
        dt.aux_hour  = aux;
      }
      break;
    case SCREEN_CLK_MIN:
      draw_string("CURRENT MINUTE", NOONE, NOONE);
      if(aux != last_aux)
      {
        if(aux < 0 || aux > 59)
        {
          aux = 0;
        }
        
        draw_big_numbers(aux, NOONE, NOONE, true);
        last_aux  = aux;
        dt.aux_min   = aux;
      }
      break;
  }
}

void buttons_push_time()
{
  button1_state = digitalRead(BUTTON1);
  button2_state = digitalRead(BUTTON2);

  if(button1_state == LOW)    // Reading LOW means that the button is pressed.
  {
    if(button1_state != button1_last_state)
    {
      button1_press_time  = millis();
      button1_last_state  = button1_state;
    }
    else
    {
      button1_pressed_time  = millis() - button1_press_time;
    }
  }
  else
  {
    if(button1_state != button1_last_state)
    {
      button1_pressed_time  = 0;
      button1_last_state    = button1_state;
    }    
  }
  
  if(button2_state == LOW)    // Reading LOW means that the button is pressed.
  {
    if(button2_state != button2_last_state)
    {
      button2_press_time  = millis();
      button2_last_state  = button2_state;
    }
    else
    {
      button2_pressed_time  = millis() - button2_press_time;
    }
  }
  else
  {
    if(button2_state != button2_last_state)
    {
      button2_pressed_time  = 0;
      button2_last_state    = button2_state;
    }    
  }
}

void check_pressed_buttons()
{
  buttons_push_time();
   
  if(button2_pressed_time >= CHG_SCR_TIME)
  {
    if(screen == SCREEN_CLK_MIN)
    {
      rtc.adjust(DateTime(dt.aux_year,dt.aux_mon,dt.aux_day,dt.aux_hour,dt.aux_min));
      screen  = SCREEN_MAIN;
    }
    else
    {
      screen++;
    }

    my_lcd.Fill_Screen(BLACK);
    button2_pressed_time  = 0;
    button2_last_state    = 1 - button2_last_state;
    aux = 0;    // Global var used to count how many times button 1 has been pressed
  }
}
