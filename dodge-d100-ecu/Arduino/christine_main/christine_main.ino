#include  "header.h"
#include  "pgm_data.h"
#include  "draw_functions.h"
#include  "parse_input.h"
#include  "core_functions.h"
//#include  "lib_sysdata.h"       // Library to manage system data stored on the EEPROM

void setup() 
{
  Serial.begin(9600);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  line1_y   = (my_lcd.Get_Display_Width() / 2 - NUMBER_HEIGHT / 2 - CHAR_HEIGHT * 2) / 3;
  line2_y   = line1_y * 2 + CHAR_HEIGHT;
  line3_y   = my_lcd.Get_Display_Width() / 2 + NUMBER_HEIGHT / 2 + line1_y;
  line4_y   = line3_y + line1_y + CHAR_HEIGHT;
  
  my_lcd.Init_LCD();
  my_lcd.Set_Rotation(1);
  my_lcd.Fill_Screen(BLACK);
Serial.println(my_lcd.Get_Display_Width());

  my_lcd.Set_Text_Mode(0);
  my_lcd.Set_Text_colour(CYAN);
  my_lcd.Set_Text_Back_colour(BLACK);
  my_lcd.Set_Text_Size(4);
  
  dht.begin();
  /*
  {
    my_lcd.Print("ERROR DEL SENSOR DHT",CENTER,my_lcd.Get_Display_Height()/2);
  }
  */
  
  if (!rtc.begin())
  {
    //Serial.println(F("Couldn't find RTC"));
    my_lcd.Print("ERROR DEL RELOJ RTC",CENTER,my_lcd.Get_Display_Height()/2);
  }
  
  // Si se ha perdido la corriente, fijar fecha y hora
  if (rtc.lostPower()) {
    // Fijar a fecha y hora de compilacion
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
    // Fijar a fecha y hora específica. En el ejemplo, 21 de Enero de 2016 a las 03:00:00
    // rtc.adjust(DateTime(2016, 1, 21, 3, 0, 0));
  }
  /*
  number_addr[0] = pgm_get_far_address(cero);
  number_addr[1] = pgm_get_far_address(uno);
  number_addr[2] = pgm_get_far_address(dos);
  number_addr[3] = pgm_get_far_address(tres);
  number_addr[4] = pgm_get_far_address(cuatro);
  number_addr[5] = pgm_get_far_address(cinco);
  number_addr[6] = pgm_get_far_address(seis);
  number_addr[7] = pgm_get_far_address(siete);
  number_addr[8] = pgm_get_far_address(ocho);
  number_addr[9] = pgm_get_far_address(nueve);

  char_addr[0]  = pgm_get_far_address(A);
  char_addr[1]  = pgm_get_far_address(B);
  char_addr[2]  = pgm_get_far_address(C);
  char_addr[3]  = pgm_get_far_address(D);
  char_addr[4]  = pgm_get_far_address(E);
  char_addr[5]  = pgm_get_far_address(F);
  char_addr[6]  = pgm_get_far_address(G);
  char_addr[7]  = pgm_get_far_address(H);
  char_addr[8]  = pgm_get_far_address(I);
  char_addr[9]  = pgm_get_far_address(J);
  char_addr[10]  = pgm_get_far_address(K);
  char_addr[11]  = pgm_get_far_address(L);
  char_addr[12]  = pgm_get_far_address(M);
  char_addr[13]  = pgm_get_far_address(N);
  char_addr[14]  = pgm_get_far_address(O);
  char_addr[15]  = pgm_get_far_address(P);
  char_addr[16]  = pgm_get_far_address(Q);
  char_addr[17]  = pgm_get_far_address(R);
  char_addr[18]  = pgm_get_far_address(S);
  char_addr[19]  = pgm_get_far_address(T);
  char_addr[20]  = pgm_get_far_address(U);
  char_addr[21]  = pgm_get_far_address(V);
  char_addr[22]  = pgm_get_far_address(W);
  char_addr[23]  = pgm_get_far_address(X);
  char_addr[24]  = pgm_get_far_address(Y);
  char_addr[25]  = pgm_get_far_address(Z);

  small_number_addr[0]  = pgm_get_far_address(c_0);
  small_number_addr[1]  = pgm_get_far_address(c_1);
  small_number_addr[2]  = pgm_get_far_address(c_2);
  small_number_addr[3]  = pgm_get_far_address(c_3);
  small_number_addr[4]  = pgm_get_far_address(c_4);
  small_number_addr[5]  = pgm_get_far_address(c_5);
  small_number_addr[6]  = pgm_get_far_address(c_6);
  small_number_addr[7]  = pgm_get_far_address(c_7);
  small_number_addr[8]  = pgm_get_far_address(c_8);
  small_number_addr[9]  = pgm_get_far_address(c_9);

  slash   = pgm_get_far_address(_slash);
  degree  = pgm_get_far_address(_degree);
  percent = pgm_get_far_address(_percent);

  temp_icon   = pgm_get_far_address(_temp);
  humdt_icon  = pgm_get_far_address(_humdt);
  car_in_out  = pgm_get_far_address(_car_in_out);
  */
  
my_lcd.Set_Draw_color(BLACK);
my_lcd.Print("Hola Mundo!",CENTER,my_lcd.Get_Display_Height()/2);
}

void loop() 
{
  /*
  check_pressed_buttons();
  
  switch(screen)
  {
    case SCREEN_MAIN:
      main_screen();
      break;
    default:
      clk_config();
      break;
  }
*/
// ----------------------------------------------------------------- //
// ------------------ LEER DATOS DEL PUERTO SERIE ------------------ //
// ----------------------------------------------------------------- //

  char      c;
  String    textBuff;
  
  while(Serial.available())
  {
    c = (char)Serial.read();
    if(c == 32 || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
    {
      textBuff += c;
      delay(5);
    }
  }

  if(textBuff.length() > 0){
    parse_input(textBuff);
  }
  
// ---------------------------------------------------------------- //
// ---------------------------------------------------------------- //
// ---------------------------------------------------------------- //

}
