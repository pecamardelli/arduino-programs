void draw_volts()
{
  int x             = 90;
  int y             = 360;
  int r_padd        = 20;
  int max_len       = my_lcd.Get_Display_Width() - x - r_padd;
  int bar_num       = 24;
  int bar_width     = max_len / bar_num;
  int bar_height    = 30;
  int lower_limit   = 9;
  int upper_limit   = 16;
  int volts_to_seg  = (volts - lower_limit) * bar_num / (upper_limit - lower_limit);
  uint16_t  color   = RED;
  
  if(volts < 12){
    color = RED;
  }
  else if(volts < 12.3){
    color = YELLOW;
  }
  else if(volts < 14.6){
    color = GREEN;
  }
  else if(volts < 15.1){
    color = YELLOW;
  }
  else{
    color = RED;
  }
      
  my_lcd.Set_Text_Size(3);
  my_lcd.Set_Text_colour(color);
  my_lcd.Set_Text_Back_colour(BLACK);
  my_lcd.Print_Number_Float(volts, 1, 5, y+5, '.', 4, ' ');
  
  for(int i=0;i<bar_num;i++){
    if(i <= volts_to_seg){
      my_lcd.Set_Draw_color(32+i*8, 32+i*8, 32+i*8);
      my_lcd.Set_Draw_color(my_lcd.Get_Draw_color()&color);
    }
    else{
      my_lcd.Set_Draw_color(BLACK); 
    }    
    my_lcd.Fill_Rectangle(x + bar_width*i, y, x + bar_width*(1+i), y + bar_height);
  }
}

void draw_amper()
{
  int x             = 90;
  int y             = 360;
  int r_padd        = 20;
  int max_len       = my_lcd.Get_Display_Width() - x - r_padd;
  int bar_num       = 24;
  int bar_width     = max_len / bar_num;
  int bar_height    = 30;
  int lower_limit   = -30;
  int upper_limit   = 30;
  int volts_to_seg  = (volts - lower_limit) * bar_num / (upper_limit - lower_limit);
  uint16_t  color   = RED;
  
  if(volts < 12){
    color = RED;
  }
  else if(volts < 12.3){
    color = YELLOW;
  }
  else if(volts < 14.6){
    color = GREEN;
  }
  else if(volts < 15.1){
    color = YELLOW;
  }
  else{
    color = RED;
  }
      
  my_lcd.Set_Text_Size(3);
  my_lcd.Set_Text_colour(color);
  my_lcd.Set_Text_Back_colour(BLACK);
  my_lcd.Print_Number_Float(volts, 1, 5, y+5, '.', 4, ' ');
  
  for(int i=0;i<bar_num;i++){
    if(i <= volts_to_seg){
      my_lcd.Set_Draw_color(32+i*8, 32+i*8, 32+i*8);
      my_lcd.Set_Draw_color(my_lcd.Get_Draw_color()&color);
    }
    else{
      my_lcd.Set_Draw_color(BLACK); 
    }    
    my_lcd.Fill_Rectangle(x + bar_width*i, y, x + bar_width*(1+i), y + bar_height);
  }
}

 /*
  int center_x  = 100;
  int center_y  = 360;
  int radius    = 80;
  int seg_num   = 36;
  int start_deg = -225;
  int end_deg   = 45;
  int dist      = 3;

  int lower_volts   = 9;
  int upper_volts   = 16;
  int volts_to_seg  = (volts - lower_volts) * seg_num / (upper_volts - lower_volts);

  float seg_len = (end_deg - start_deg) / seg_num;

  int x0  = 0;
  int y0  = 0;
  int x2  = 0;
  int y2  = 0;
  
  float current_deg = 0;
  
  for(int i=0;i<volts_to_seg;i++){
    current_deg = start_deg + i * seg_len;
    x0  = center_x + radius * cos(DEG_TO_RAD * current_deg);
    y0  = center_y + radius * sin(DEG_TO_RAD * current_deg);
    x2  = center_x + radius * cos(DEG_TO_RAD * (current_deg + seg_len - dist));
    y2  = center_y + radius * sin(DEG_TO_RAD * (current_deg + seg_len - dist));

    my_lcd.Set_Draw_color(RED);
    my_lcd.Fill_Triangle(x0,y0,center_x,center_y,x2,y2);
  }

  my_lcd.Set_Draw_color(BLACK);
  my_lcd.Fill_Circle(center_x,center_y,radius*0.7);

  my_lcd.Set_Text_Size(3);
  my_lcd.Set_Text_colour(WHITE);
  my_lcd.Set_Text_Back_colour(BLACK);
  my_lcd.Print_Number_Float(volts, 1, center_x - radius/3, center_y - radius/4, '.', 0, ' ');
  */
