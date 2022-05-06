#ifndef _HEADER_H
#define _HEADER_H

  #include <SD.h>
  #include <SPI.h>
  #include <LCDWIKI_GUI.h> //Core graphics library
  #include <LCDWIKI_KBV.h> //Hardware-specific library
  #include <avr/pgmspace.h>
  #include <Wire.h>
  #include "RTClib.h"
  #include <EEPROM.h>
  #include <DHT.h>
  #include <DHT_U.h>
  
#include <EEPROM.h>
  
  // Algunos colores predefinidos
  #define BLACK   0x0000
  #define BLUE    0x001F
  #define RED     0xF800
  #define GREEN   0x07E0
  #define CYAN    0x07FF
  #define MAGENTA 0xF81F
  #define YELLOW  0xFFE0
  #define WHITE   0xFFFF
  
  #define NUMBER_WIDTH  60
  #define NUMBER_HEIGHT 90
  #define DOTS_WIDTH    30
  #define DOTS_HEIGHT   90
  #define CHAR_WIDTH    30
  #define CHAR_HEIGHT   30
  #define SYMBOL_WIDTH  60
  #define SYMBOL_HEIGHT 60
  
  #define DHTPIN    2         // Digital pin connected to the DHT sensor
  #define DHTTYPE   DHT11     // DHT 11
  DHT_Unified dht(DHTPIN, DHTTYPE);
  
  #define NOONE           999
  #define SCREEN_MAIN     0
  #define SCREEN_CLK_YEAR 1
  #define SCREEN_CLK_MON  2
  #define SCREEN_CLK_DAY  3
  #define SCREEN_CLK_HOUR 4
  #define SCREEN_CLK_MIN  5
  #define CHG_SCR_TIME    3000
  
  #define BUTTON1   3
  #define BUTTON2   4
  #define BUZZER    19

  // Definición de la sección .fini1 para almacenar todos los datos
  // en la memoria flash. De esta forma, toda la data que pongamos
  // en esta sección quedará en una zona superior de la memoria, dejando
  // la parte inferior para el programa.
  #define PROGMEM_LATE1 __attribute__ (( __section__(".fini1") ))
  
  //the definiens of 16bit mode as follow:
  //if the IC model is known or the modules is unreadable,you can use this constructed function
  LCDWIKI_KBV my_lcd(ILI9486,40,38,39,-1,41); //model,cs,cd,wr,rd,reset
  
  uint32_t  number_addr[10];        // Array con las direcciones de los mapas de bits de los números
  uint32_t  char_addr[26];          // Array con las direcciones de los mapas de bits de las letras
  uint32_t  small_number_addr[10];  // Array con las direcciones de los mapas de bits de los números chicos
  uint32_t  slash, degree, percent,temp_icon,humdt_icon,car_in_out;
  uint8_t   last_sec      = 0;
  uint8_t   current_day   = 255;
  uint16_t  last_temp     = NOONE;
  uint16_t  last_humdt    = NOONE;
  bool      printing_msg  = false;
  String    days[]        = { "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado" };
  uint16_t  line1_y       = 0;
  uint16_t  line2_y       = 0;
  uint16_t  line3_y       = 0;
  uint16_t  line4_y       = 0;
  
  int           button1_state         = HIGH;
  int           button1_last_state    = HIGH;
  unsigned long button1_press_time    = 0;
  unsigned long button1_pressed_time  = 0;
  int           button2_state         = HIGH;
  int           button2_last_state    = HIGH;
  unsigned long button2_press_time    = 0;
  unsigned long button2_pressed_time  = 0;
  
  int           screen                = SCREEN_MAIN;
  int           aux                   = 0;
  int           last_aux              = NOONE;
  
  #define   LINE_PADDING 20        // Separación entre el espacio para los caracteres y los bordes de la pantalla
  #define   MAX_LINE_LEN ((my_lcd.Get_Display_Width() - LINE_PADDING) / CHAR_WIDTH)     // Cálculo de la cantidad máxima de caracteres por renglón
  
  RTC_DS3231 rtc;
  
  typedef struct _dt{
    uint16_t  aux_year;
    uint8_t   aux_mon;
    uint8_t   aux_day;
    uint8_t   aux_hour;
    uint8_t   aux_min;
  };
  
  _dt dt;  
  
  // ---------- FUNCTION DEFINITION ------------ //
  
  void    draw_bitmap           (uint32_t, uint16_t, uint16_t, uint16_t, uint16_t, bool);
  void    draw_string           (const char *, uint16_t, uint16_t);
  void    draw_string           (String, uint16_t, uint16_t);
  void    draw_date_time        ();
  void    draw_big_numbers      (int,uint16_t, uint16_t, bool);
  void    parse_input           (String);
  void    reset_message         ();
  int     get_temp              ();
  int     get_humdt             ();
  void    draw_temp             ();
  void    draw_temp_icons       ();
  void    main_screen           ();
  void    clk_config            ();
  void    buttons_push_time     ();
  void    check_pressed_buttons ();
  
#endif // _HEADER_H
