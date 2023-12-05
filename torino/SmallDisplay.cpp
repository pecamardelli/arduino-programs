// Example code from https://electropeak.com/learn/interfacing-sh1106-1-3-inch-i2c-oled-128x64-display-with-arduino/#google_vignette

#include "SmallDisplay.h"

Adafruit_SH1106 smallDisplay(OLED_RESET);

void drawTorinoLogo()
{
    // Clear the buffer.
    smallDisplay.clearDisplay();
    smallDisplay.drawBitmap(0, 0, logo16_glcd_bmp, 16, 16, WHITE);
    smallDisplay.display();
}

void drawFuelConsumption(float value)
{
    if (value > 10)
        smallDisplay.setCursor(10, 0);
    else
        smallDisplay.setCursor(28, 0);

    smallDisplay.clearDisplay();
    smallDisplay.setTextSize(3);
    smallDisplay.setTextColor(WHITE);
    smallDisplay.print(value, 2);
    smallDisplay.setCursor(110, 0);
    smallDisplay.setTextSize(1);
    smallDisplay.print("KM");
    smallDisplay.setCursor(110, 12);
    smallDisplay.println("LT");
    smallDisplay.display();
}

void draw()
{
    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    smallDisplay.display();
    delay(2000);

    // Clear the buffer.
    smallDisplay.clearDisplay();

    // draw a single pixel
    smallDisplay.drawPixel(10, 10, WHITE);
    // Show the display buffer on the hardware.
    // NOTE: You _must_ call display after making any drawing commands
    // to make them visible on the display hardware!
    smallDisplay.display();
    delay(2000);
    smallDisplay.clearDisplay();

    // draw many lines
    testdrawline();
    smallDisplay.display();
    delay(2000);
    smallDisplay.clearDisplay();

    // draw rectangles
    testdrawrect();
    smallDisplay.display();
    delay(2000);
    smallDisplay.clearDisplay();

    // draw multiple rectangles
    testfillrect();
    smallDisplay.display();
    delay(2000);
    smallDisplay.clearDisplay();

    // draw mulitple circles
    testdrawcircle();
    smallDisplay.display();
    delay(2000);
    smallDisplay.clearDisplay();

    // draw a white circle, 10 pixel radius
    smallDisplay.fillCircle(smallDisplay.width() / 2, smallDisplay.height() / 2, 10, WHITE);
    smallDisplay.display();
    delay(2000);
    smallDisplay.clearDisplay();

    testdrawroundrect();
    delay(2000);
    smallDisplay.clearDisplay();

    testfillroundrect();
    delay(2000);
    smallDisplay.clearDisplay();

    testdrawtriangle();
    delay(2000);
    smallDisplay.clearDisplay();

    testfilltriangle();
    delay(2000);
    smallDisplay.clearDisplay();

    // draw the first ~12 characters in the font
    testdrawchar();
    smallDisplay.display();
    delay(2000);
    smallDisplay.clearDisplay();

    // draw scrolling text
    /* testscrolltext();
     delay(2000);
     display.clearDisplay();*/

    // text display tests
    smallDisplay.setTextSize(1);
    smallDisplay.setTextColor(WHITE);
    smallDisplay.setCursor(0, 0);
    smallDisplay.println("Hello, world!");
    smallDisplay.setTextColor(BLACK, WHITE); // 'inverted' text
    smallDisplay.println(3.141592);
    smallDisplay.setTextSize(2);
    smallDisplay.setTextColor(WHITE);
    smallDisplay.print("0x");
    smallDisplay.println(0xDEADBEEF, HEX);
    smallDisplay.display();
    delay(2000);

    // miniature bitmap display
    smallDisplay.clearDisplay();
    smallDisplay.drawBitmap(30, 16, logo16_glcd_bmp, 16, 16, 1);
    smallDisplay.display();

    // invert the display
    smallDisplay.invertDisplay(true);
    delay(1000);
    smallDisplay.invertDisplay(false);
    delay(1000);

    // draw a bitmap icon and 'animate' movement
    testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h)
{
    uint8_t icons[NUMFLAKES][3];

    // initialize
    for (uint8_t f = 0; f < NUMFLAKES; f++)
    {
        icons[f][XPOS] = random(smallDisplay.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;

        Serial.print("x: ");
        Serial.print(icons[f][XPOS], DEC);
        Serial.print(" y: ");
        Serial.print(icons[f][YPOS], DEC);
        Serial.print(" dy: ");
        Serial.println(icons[f][DELTAY], DEC);
    }

    while (1)
    {
        // draw each icon
        for (uint8_t f = 0; f < NUMFLAKES; f++)
        {
            smallDisplay.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, WHITE);
        }
        smallDisplay.display();
        delay(200);

        // then erase it + move it
        for (uint8_t f = 0; f < NUMFLAKES; f++)
        {
            smallDisplay.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, BLACK);
            // move it
            icons[f][YPOS] += icons[f][DELTAY];
            // if its gone, reinit
            if (icons[f][YPOS] > smallDisplay.height())
            {
                icons[f][XPOS] = random(smallDisplay.width());
                icons[f][YPOS] = 0;
                icons[f][DELTAY] = random(5) + 1;
            }
        }
    }
}

void testdrawchar(void)
{
    smallDisplay.setTextSize(1);
    smallDisplay.setTextColor(WHITE);
    smallDisplay.setCursor(0, 0);

    for (uint8_t i = 0; i < 168; i++)
    {
        if (i == '\n')
            continue;
        smallDisplay.write(i);
        if ((i > 0) && (i % 21 == 0))
            smallDisplay.println();
    }
    smallDisplay.display();
}

void testdrawcircle(void)
{
    for (int16_t i = 0; i < smallDisplay.height(); i += 2)
    {
        smallDisplay.drawCircle(smallDisplay.width() / 2, smallDisplay.height() / 2, i, WHITE);
        smallDisplay.display();
    }
}

void testfillrect(void)
{
    uint8_t color = 1;
    for (int16_t i = 0; i < smallDisplay.height() / 2; i += 3)
    {
        // alternate colors
        smallDisplay.fillRect(i, i, smallDisplay.width() - i * 2, smallDisplay.height() - i * 2, color % 2);
        smallDisplay.display();
        color++;
    }
}

void testdrawtriangle(void)
{
    for (int16_t i = 0; i < min(smallDisplay.width(), smallDisplay.height()) / 2; i += 5)
    {
        smallDisplay.drawTriangle(smallDisplay.width() / 2, smallDisplay.height() / 2 - i,
                                  smallDisplay.width() / 2 - i, smallDisplay.height() / 2 + i,
                                  smallDisplay.width() / 2 + i, smallDisplay.height() / 2 + i, WHITE);
        smallDisplay.display();
    }
}

void testfilltriangle(void)
{
    uint8_t color = WHITE;
    for (int16_t i = min(smallDisplay.width(), smallDisplay.height()) / 2; i > 0; i -= 5)
    {
        smallDisplay.fillTriangle(smallDisplay.width() / 2, smallDisplay.height() / 2 - i,
                                  smallDisplay.width() / 2 - i, smallDisplay.height() / 2 + i,
                                  smallDisplay.width() / 2 + i, smallDisplay.height() / 2 + i, WHITE);
        if (color == WHITE)
            color = BLACK;
        else
            color = WHITE;
        smallDisplay.display();
    }
}

void testdrawroundrect(void)
{
    for (int16_t i = 0; i < smallDisplay.height() / 2 - 2; i += 2)
    {
        smallDisplay.drawRoundRect(i, i, smallDisplay.width() - 2 * i, smallDisplay.height() - 2 * i, smallDisplay.height() / 4, WHITE);
        smallDisplay.display();
    }
}

void testfillroundrect(void)
{
    uint8_t color = WHITE;
    for (int16_t i = 0; i < smallDisplay.height() / 2 - 2; i += 2)
    {
        smallDisplay.fillRoundRect(i, i, smallDisplay.width() - 2 * i, smallDisplay.height() - 2 * i, smallDisplay.height() / 4, color);
        if (color == WHITE)
            color = BLACK;
        else
            color = WHITE;
        smallDisplay.display();
    }
}

void testdrawrect(void)
{
    for (int16_t i = 0; i < smallDisplay.height() / 2; i += 2)
    {
        smallDisplay.drawRect(i, i, smallDisplay.width() - 2 * i, smallDisplay.height() - 2 * i, WHITE);
        smallDisplay.display();
    }
}

void testdrawline()
{
    for (int16_t i = 0; i < smallDisplay.width(); i += 4)
    {
        smallDisplay.drawLine(0, 0, i, smallDisplay.height() - 1, WHITE);
        smallDisplay.display();
    }
    for (int16_t i = 0; i < smallDisplay.height(); i += 4)
    {
        smallDisplay.drawLine(0, 0, smallDisplay.width() - 1, i, WHITE);
        smallDisplay.display();
    }
    delay(250);

    smallDisplay.clearDisplay();
    for (int16_t i = 0; i < smallDisplay.width(); i += 4)
    {
        smallDisplay.drawLine(0, smallDisplay.height() - 1, i, 0, WHITE);
        smallDisplay.display();
    }
    for (int16_t i = smallDisplay.height() - 1; i >= 0; i -= 4)
    {
        smallDisplay.drawLine(0, smallDisplay.height() - 1, smallDisplay.width() - 1, i, WHITE);
        smallDisplay.display();
    }
    delay(250);

    smallDisplay.clearDisplay();
    for (int16_t i = smallDisplay.width() - 1; i >= 0; i -= 4)
    {
        smallDisplay.drawLine(smallDisplay.width() - 1, smallDisplay.height() - 1, i, 0, WHITE);
        smallDisplay.display();
    }
    for (int16_t i = smallDisplay.height() - 1; i >= 0; i -= 4)
    {
        smallDisplay.drawLine(smallDisplay.width() - 1, smallDisplay.height() - 1, 0, i, WHITE);
        smallDisplay.display();
    }
    delay(250);

    smallDisplay.clearDisplay();
    for (int16_t i = 0; i < smallDisplay.height(); i += 4)
    {
        smallDisplay.drawLine(smallDisplay.width() - 1, 0, 0, i, WHITE);
        smallDisplay.display();
    }
    for (int16_t i = 0; i < smallDisplay.width(); i += 4)
    {
        smallDisplay.drawLine(smallDisplay.width() - 1, 0, i, smallDisplay.height() - 1, WHITE);
        smallDisplay.display();
    }
    delay(250);
}

/*void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("scroll");
  display.display();

  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}*/