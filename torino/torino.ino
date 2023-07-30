// IMPORTANT: LCDWIKI_KBV LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.

// This program is a demo of displaying string
// Set the pins to the correct ones for your development shield or breakout board.
// the 16bit mode only use in Mega.you must modify the mode in the file of lcd_mode.h
// when using the BREAKOUT BOARD only and using these 16 data lines to the LCD,
// pin usage as follow:
//              CS  CD  WR  RD  RST  D0  D1  D2  D3  D4  D5  D6  D7  D8  D9  D10  D11  D12  D13  D14  D15
// Arduino Mega 40  38  39  /   41   37  36  35  34  33  32  31  30  22  23  24   25   26   27   28   29

// Remember to set the pins to suit your display module!

/**********************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 **********************************************************************************/

#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library

// the definiens of 16bit mode as follow:
// if the IC model is known or the modules is unreadable,you can use this constructed function
LCDWIKI_KBV mylcd(ILI9486, 40, 38, 39, -1, 41); // model,cs,cd,wr,rd,reset

// define some colour values
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

void setup()
{
    mylcd.Init_LCD();
    mylcd.Fill_Screen(BLACK);

    mylcd.Set_Rotation(1);
    mylcd.Set_Text_Mode(0);
    mylcd.Set_Text_colour(CYAN);
    mylcd.Set_Text_Back_colour(BLACK);
    mylcd.Set_Text_Size(9);
    char var[] = "12:39";
    mylcd.Print((uint8_t *)var, CENTER, 100);

    mylcd.Set_Text_Size(3);
    char var2[] = "EXT 34c";
    mylcd.Print((uint8_t *)var2, CENTER, 280);

    Serial.begin(9600);
    Serial.println("Initializing...");
}

void loop()
{
}
