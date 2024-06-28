// Example code from https://electropeak.com/learn/interfacing-sh1106-1-3-inch-i2c-oled-128x64-display-with-arduino/#google_vignette

#include "SmallDisplay.h"

Adafruit_SH1106 oled(OLED_RESET);

SmallDisplay::SmallDisplay(/* args */)
{
    lastMillis = 0;
    refreshInterval = 1000; // Amount of milliseconds between display refreshes.
}

SmallDisplay::~SmallDisplay()
{
}

void SmallDisplay::begin()
{
    // By default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    oled.begin(SH1106_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
    // Init done
    // smallDisplay.clearDisplay();
    oled.display();

    Serial.print("Display width: ");
    Serial.println(oled.width());
    Serial.print("Display height: ");
    Serial.println(oled.height());
}

void SmallDisplay::display(String date, String time, String temp, String humidity)
{
    currentMillis = millis();

    if (lastMillis + refreshInterval > currentMillis)
    {
        return;
    }

    oled.clearDisplay();
    oled.setTextColor(WHITE);

    // Time
    oled.setCursor(defaultTimeDisplayX, defaultTimeDisplayY);
    oled.setTextSize(defaultTimeDisplaySize);
    oled.print(time);

    // Date
    uint8_t fullDateX = (21 - date.length()) * 3;
    oled.setCursor(fullDateX, defaultDateDisplayY);
    oled.setTextSize(defaultDateDisplaySize);
    oled.print(date);

    oled.setCursor(defaultTempDisplayX, defaultTempDisplayY);
    oled.setTextSize(defaultTempDisplaySize);
    oled.print(temp);

    // Humidity
    oled.setCursor(defaultHumdtDisplayX, defaultHumdtDisplayY);
    oled.setTextSize(defaultHumdtDisplaySize);
    oled.print(humidity);

    oled.display();

    lastMillis = currentMillis;
}

void SmallDisplay::drawTorinoLogo()
{
    // Clear the buffer.
    oled.clearDisplay();
    oled.drawBitmap(0, 0, logo16_glcd_bmp, 16, 16, WHITE);
    oled.display();
}

void drawFuelConsumption(float value)
{
    if (value > 10)
        oled.setCursor(10, 0);
    else
        oled.setCursor(28, 0);

    oled.clearDisplay();
    oled.setTextSize(3);
    oled.setTextColor(WHITE);
    oled.print(value, 2);
    oled.setCursor(110, 0);
    oled.setTextSize(1);
    oled.print("KM");
    oled.setCursor(110, 12);
    oled.println("LT");
    oled.display();
}

void draw()
{
    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    oled.display();
    delay(2000);

    // Clear the buffer.
    oled.clearDisplay();

    // draw a single pixel
    oled.drawPixel(10, 10, WHITE);
    // Show the display buffer on the hardware.
    // NOTE: You _must_ call display after making any drawing commands
    // to make them visible on the display hardware!
    oled.display();
    delay(2000);
    oled.clearDisplay();

    // draw many lines
    testdrawline();
    oled.display();
    delay(2000);
    oled.clearDisplay();

    // draw rectangles
    testdrawrect();
    oled.display();
    delay(2000);
    oled.clearDisplay();

    // draw multiple rectangles
    testfillrect();
    oled.display();
    delay(2000);
    oled.clearDisplay();

    // draw mulitple circles
    testdrawcircle();
    oled.display();
    delay(2000);
    oled.clearDisplay();

    // draw a white circle, 10 pixel radius
    oled.fillCircle(oled.width() / 2, oled.height() / 2, 10, WHITE);
    oled.display();
    delay(2000);
    oled.clearDisplay();

    testdrawroundrect();
    delay(2000);
    oled.clearDisplay();

    testfillroundrect();
    delay(2000);
    oled.clearDisplay();

    testdrawtriangle();
    delay(2000);
    oled.clearDisplay();

    testfilltriangle();
    delay(2000);
    oled.clearDisplay();

    // draw the first ~12 characters in the font
    testdrawchar();
    oled.display();
    delay(2000);
    oled.clearDisplay();

    // draw scrolling text
    /* testscrolltext();
     delay(2000);
     display.clearDisplay();*/

    // text display tests
    oled.setTextSize(1);
    oled.setTextColor(WHITE);
    oled.setCursor(0, 0);
    oled.println("Hello, world!");
    oled.setTextColor(BLACK, WHITE); // 'inverted' text
    oled.println(3.141592);
    oled.setTextSize(2);
    oled.setTextColor(WHITE);
    oled.print("0x");
    oled.println(0xDEADBEEF, HEX);
    oled.display();
    delay(2000);

    // miniature bitmap display
    oled.clearDisplay();
    oled.drawBitmap(30, 16, logo16_glcd_bmp, 16, 16, 1);
    oled.display();

    // invert the display
    oled.invertDisplay(true);
    delay(1000);
    oled.invertDisplay(false);
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
        icons[f][XPOS] = random(oled.width());
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
            oled.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, WHITE);
        }
        oled.display();
        delay(200);

        // then erase it + move it
        for (uint8_t f = 0; f < NUMFLAKES; f++)
        {
            oled.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, BLACK);
            // move it
            icons[f][YPOS] += icons[f][DELTAY];
            // if its gone, reinit
            if (icons[f][YPOS] > oled.height())
            {
                icons[f][XPOS] = random(oled.width());
                icons[f][YPOS] = 0;
                icons[f][DELTAY] = random(5) + 1;
            }
        }
    }
}

void testdrawchar(void)
{
    oled.setTextSize(1);
    oled.setTextColor(WHITE);
    oled.setCursor(0, 0);

    for (uint8_t i = 0; i < 168; i++)
    {
        if (i == '\n')
            continue;
        oled.write(i);
        if ((i > 0) && (i % 21 == 0))
            oled.println();
    }
    oled.display();
}

void testdrawcircle(void)
{
    for (int16_t i = 0; i < oled.height(); i += 2)
    {
        oled.drawCircle(oled.width() / 2, oled.height() / 2, i, WHITE);
        oled.display();
    }
}

void testfillrect(void)
{
    uint8_t color = 1;
    for (int16_t i = 0; i < oled.height() / 2; i += 3)
    {
        // alternate colors
        oled.fillRect(i, i, oled.width() - i * 2, oled.height() - i * 2, color % 2);
        oled.display();
        color++;
    }
}

void testdrawtriangle(void)
{
    for (int16_t i = 0; i < min(oled.width(), oled.height()) / 2; i += 5)
    {
        oled.drawTriangle(oled.width() / 2, oled.height() / 2 - i,
                          oled.width() / 2 - i, oled.height() / 2 + i,
                          oled.width() / 2 + i, oled.height() / 2 + i, WHITE);
        oled.display();
    }
}

void testfilltriangle(void)
{
    uint8_t color = WHITE;
    for (int16_t i = min(oled.width(), oled.height()) / 2; i > 0; i -= 5)
    {
        oled.fillTriangle(oled.width() / 2, oled.height() / 2 - i,
                          oled.width() / 2 - i, oled.height() / 2 + i,
                          oled.width() / 2 + i, oled.height() / 2 + i, WHITE);
        if (color == WHITE)
            color = BLACK;
        else
            color = WHITE;
        oled.display();
    }
}

void testdrawroundrect(void)
{
    for (int16_t i = 0; i < oled.height() / 2 - 2; i += 2)
    {
        oled.drawRoundRect(i, i, oled.width() - 2 * i, oled.height() - 2 * i, oled.height() / 4, WHITE);
        oled.display();
    }
}

void testfillroundrect(void)
{
    uint8_t color = WHITE;
    for (int16_t i = 0; i < oled.height() / 2 - 2; i += 2)
    {
        oled.fillRoundRect(i, i, oled.width() - 2 * i, oled.height() - 2 * i, oled.height() / 4, color);
        if (color == WHITE)
            color = BLACK;
        else
            color = WHITE;
        oled.display();
    }
}

void testdrawrect(void)
{
    for (int16_t i = 0; i < oled.height() / 2; i += 2)
    {
        oled.drawRect(i, i, oled.width() - 2 * i, oled.height() - 2 * i, WHITE);
        oled.display();
    }
}

void testdrawline()
{
    for (int16_t i = 0; i < oled.width(); i += 4)
    {
        oled.drawLine(0, 0, i, oled.height() - 1, WHITE);
        oled.display();
    }
    for (int16_t i = 0; i < oled.height(); i += 4)
    {
        oled.drawLine(0, 0, oled.width() - 1, i, WHITE);
        oled.display();
    }
    delay(250);

    oled.clearDisplay();
    for (int16_t i = 0; i < oled.width(); i += 4)
    {
        oled.drawLine(0, oled.height() - 1, i, 0, WHITE);
        oled.display();
    }
    for (int16_t i = oled.height() - 1; i >= 0; i -= 4)
    {
        oled.drawLine(0, oled.height() - 1, oled.width() - 1, i, WHITE);
        oled.display();
    }
    delay(250);

    oled.clearDisplay();
    for (int16_t i = oled.width() - 1; i >= 0; i -= 4)
    {
        oled.drawLine(oled.width() - 1, oled.height() - 1, i, 0, WHITE);
        oled.display();
    }
    for (int16_t i = oled.height() - 1; i >= 0; i -= 4)
    {
        oled.drawLine(oled.width() - 1, oled.height() - 1, 0, i, WHITE);
        oled.display();
    }
    delay(250);

    oled.clearDisplay();
    for (int16_t i = 0; i < oled.height(); i += 4)
    {
        oled.drawLine(oled.width() - 1, 0, 0, i, WHITE);
        oled.display();
    }
    for (int16_t i = 0; i < oled.width(); i += 4)
    {
        oled.drawLine(oled.width() - 1, 0, i, oled.height() - 1, WHITE);
        oled.display();
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