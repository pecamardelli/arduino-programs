void draw_bitmap(uint32_t addr, uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, bool draw)
{
  int x2  = x1 + width - 1;
  int y2  = y1 + height - 1;
  
  if(draw)
  {
    my_lcd.Set_Addr_Window(x1,y1,x2,y2);
    // La función Push_Colors es una modificación de Push_Any_Colors
    // para que lea los datos de la flash con direcciones de 32 bits.
    // No está originalmente en la librería LCDWIKI_KBV.h
    my_lcd.Push_Colors(addr, width*height,1,1); 
  }
  else
  {
    my_lcd.Set_Draw_color(BLACK);
    my_lcd.Fill_Rectangle(x1,y1,x2,y2);
  }
}

void draw_date_time()
{
  DateTime now = rtc.now();
  //my_lcd.Set_Text_Size(2);
  //my_lcd.Print("Couldn't find RTC",CENTER,0);
  
  uint16_t  _hora   = now.hour();
  uint16_t  _min    = now.minute();
  uint16_t  _seg    = now.second();
  uint16_t  aux     = 0;

  if(_seg == last_sec)
  {
    return;
  }
  else
  {
    last_sec = _seg;
  }
  
  uint16_t  x       = my_lcd.Get_Display_Width()  / 2 - DOTS_WIDTH  / 2 - NUMBER_WIDTH  * 2;
  uint16_t  y       = my_lcd.Get_Display_Height() / 2 - DOTS_HEIGHT / 2;

  if(!printing_msg && now.dayOfTheWeek() != current_day)
  {
    reset_message();
    char date[20];
    //my_lcd.Set_Draw_color(BLACK);
    //my_lcd.Fill_Rectangle(0,line1_y,my_lcd.Get_Display_Height(),line1_y + CHAR_HEIGHT * 2);
    draw_string(days[now.dayOfTheWeek()],NOONE,line1_y);
    sprintf(date,"%d/%d/%d",now.day(),now.month(),now.year());
    draw_string(date, NOONE, line2_y);
    current_day = now.dayOfTheWeek();
  }

  while(_hora > 9)
  {
    aux++;
    _hora -= 10;
  }
  
  draw_bitmap(number_addr[aux],x,y,NUMBER_WIDTH,NUMBER_HEIGHT,true);
  x += NUMBER_WIDTH;
  draw_bitmap(number_addr[_hora],x,y,NUMBER_WIDTH,NUMBER_HEIGHT,true);
  x += NUMBER_WIDTH;
  aux = 0;

  if(_seg%2 == 0)
  {
    draw_bitmap(pgm_get_far_address(dots),x,y,DOTS_WIDTH,DOTS_HEIGHT,true);
  }
  else
  {
    //draw_bitmap(pgm_get_far_address(dots),x,y,DOTS_WIDTH,DOTS_HEIGHT,false);
    my_lcd.Set_Draw_color(BLACK);
    my_lcd.Fill_Rectangle(x,y, x + DOTS_WIDTH, y + DOTS_HEIGHT);
  }
  x += DOTS_WIDTH;
  
  while(_min > 9)
  {
    aux++;
    _min -= 10;
  }
  
  draw_bitmap(number_addr[aux],x,y,NUMBER_WIDTH,NUMBER_HEIGHT,true);
  x += NUMBER_WIDTH;
  draw_bitmap(number_addr[_min],x,y,NUMBER_WIDTH,NUMBER_HEIGHT,true);
  x += NUMBER_WIDTH;
  aux = 0;

  /*
  draw_bitmap(pgm_get_far_address(dots),x,y,true);
  x += DOTS_WIDTH;

  while(_seg > 9){
    aux++;
    _seg -= 10;
  }
  
  draw_bitmap(number_addr[aux],x,y,true);
  x += NUMBER_WIDTH;
  draw_bitmap(number_addr[_seg],x,y,true);
  */
}

void draw_string(String string, uint16_t x, uint16_t y)
{
  char buf[string.length()+1];
  string.toCharArray(buf, string.length()+1);
  draw_string(buf,x,y);
}

void draw_string(const char *string, uint16_t x, uint16_t y)
{
  size_t    len = strlen(string);
  byte      last_space = 0;
  //uint16_t  x,y;
  uint16_t  chars_per_line  = (my_lcd.Get_Display_Width() - LINE_PADDING) / CHAR_WIDTH;

  if(x == NOONE)
  {
    if(len > chars_per_line)
    {
      for(byte k = 0; k < len; k++)
      {
        if(string[k] == 32)
        {
          last_space  = k;
        }
  
        if(k == (chars_per_line - 1))
        {
          break;
        }
      }
      x = my_lcd.Get_Display_Width() / 2 - last_space * CHAR_WIDTH / 2;
    }
    else
    {
      x = my_lcd.Get_Display_Width() / 2 - len * CHAR_WIDTH / 2;
    }
  }

  if(y == NOONE)
  {
    if(len <= MAX_LINE_LEN)
    {
      y = (my_lcd.Get_Display_Height() / 2 - NUMBER_HEIGHT / 2 - CHAR_HEIGHT) / 2;
    }
    else
    {
      y = (my_lcd.Get_Display_Height() / 2 - NUMBER_HEIGHT / 2 - CHAR_HEIGHT * 2) / 3;
    }
  }
  
  for(byte i = 0; i < len; i++)
  {
    if(string[i] >= 65 && string[i] <= 90)
    {
      //Serial.println((int)string[i]);
      draw_bitmap(char_addr[(int)string[i] - 65],x,y,CHAR_WIDTH,CHAR_HEIGHT,true);
    }
    else if(string[i] >= 97 && string[i] <= 122)
    {
      draw_bitmap(char_addr[(int)string[i] - 97],x,y,CHAR_WIDTH,CHAR_HEIGHT,true);
    }
    else if(string[i] >= 48 && string[i] <= 57)
    {
      draw_bitmap(small_number_addr[(int)string[i] - 48],x,y,CHAR_WIDTH,CHAR_HEIGHT,true);
    }
    else if(string[i] == 47)
    {
      draw_bitmap(slash,x,y,CHAR_WIDTH,CHAR_HEIGHT,true);
    }
    
    x += CHAR_WIDTH;

    //if(i == chars_per_line && y < CHAR_HEIGHT && last_space > 0){
    if(i == last_space && len > MAX_LINE_LEN)
    {
      y = (y*2) + CHAR_HEIGHT;
      x = my_lcd.Get_Display_Width() / 2 - (len - last_space - 1) * CHAR_WIDTH / 2;
    }
  }
}

void reset_message()
{
  my_lcd.Set_Draw_color(BLACK);
  my_lcd.Fill_Rectangle(0,0, my_lcd.Get_Display_Width(), my_lcd.Get_Display_Height() / 2 - NUMBER_HEIGHT / 2);
}

void draw_temp()
{
  char      c[3];
  int       t = get_temp();
  int       h = get_humdt();
  uint16_t  x = my_lcd.Get_Display_Width() / 2 - CHAR_WIDTH * 3 - SYMBOL_WIDTH /2;
  
  /*
  uint16_t  y = my_lcd.Get_Display_Height() / 2 + NUMBER_HEIGHT / 2;
  y = (my_lcd.Get_Display_Height() - y - SYMBOL_HEIGHT) / 2 + y;  // Esto es magia.
  */
  
  if(last_temp != t)
  {
    if(t < 10)
    {
      my_lcd.Set_Draw_color(BLACK);
      my_lcd.Fill_Rectangle(x,line3_y, x + CHAR_WIDTH, line3_y + CHAR_HEIGHT);
      x += CHAR_WIDTH;
    }

    sprintf(c,"%d",t);
    draw_string(c,x,line3_y);
    last_temp = t;
  }

  if(last_humdt != h)
  {
    if(h < 10)
    {
      x = my_lcd.Get_Display_Width() / 2 + SYMBOL_WIDTH / 2 + CHAR_WIDTH;
      my_lcd.Set_Draw_color(BLACK);
      my_lcd.Fill_Rectangle(x - CHAR_WIDTH,line3_y, x, line3_y + CHAR_HEIGHT);
    }
    else
    {
      x = my_lcd.Get_Display_Width() / 2 + SYMBOL_WIDTH / 2;
    }
  
    sprintf(c,"%d",h);
    draw_string(c,x,line3_y);
    last_humdt  = h;
  }
}

void draw_temp_icons()
{
  uint16_t  x = my_lcd.Get_Display_Width() / 2 - CHAR_WIDTH * 3 - SYMBOL_WIDTH * 1.5;
  
  /*
  uint16_t  y = my_lcd.Get_Display_Height() / 2 + NUMBER_HEIGHT / 2;
  y = (my_lcd.Get_Display_Height() - y - SYMBOL_HEIGHT) / 2 + y;  // Esto es magia.
  */
  
  draw_bitmap(temp_icon,x,line3_y,SYMBOL_WIDTH,SYMBOL_HEIGHT,true);

  x = my_lcd.Get_Display_Width() / 2 - CHAR_WIDTH - SYMBOL_WIDTH / 2;

  draw_bitmap(degree,x,line3_y,CHAR_WIDTH,CHAR_HEIGHT,true);

  x = my_lcd.Get_Display_Width() / 2 - SYMBOL_WIDTH / 2;
  
  draw_bitmap(car_in_out,x,line3_y,SYMBOL_WIDTH,SYMBOL_HEIGHT,true);

  x = my_lcd.Get_Display_Width() / 2 + SYMBOL_WIDTH / 2 + CHAR_WIDTH * 2;

  draw_bitmap(percent,x,line3_y,CHAR_WIDTH,CHAR_HEIGHT,true);

  x = my_lcd.Get_Display_Width() / 2 + SYMBOL_WIDTH / 2 + CHAR_WIDTH * 3;

  draw_bitmap(humdt_icon,x,line3_y,SYMBOL_WIDTH,SYMBOL_HEIGHT,true);
}

void draw_big_numbers(int number, uint16_t x, uint16_t y, bool draw_zero)
{
  char      aux[5];

  if(number < 10 && draw_zero)
  {
    sprintf(aux,"0%d",number);
  }
  else
  {
    sprintf(aux,"%d",number);
  }

  int       digits  = strlen(aux);
  int       aux2;

  if(x == NOONE)
  {
    x = my_lcd.Get_Display_Width() / 2 - NUMBER_WIDTH * 0.5 * digits;
  }

  if(y == NOONE)
  {
    y = my_lcd.Get_Display_Height()  / 2 - NUMBER_HEIGHT / 2;
  }

  for(uint8_t i = 0; i < digits; i++)
  {
    aux2  = aux[i] - 48;
    draw_bitmap(number_addr[aux2],x,y,NUMBER_WIDTH,NUMBER_HEIGHT,true);
    x += NUMBER_WIDTH;
  }
}
