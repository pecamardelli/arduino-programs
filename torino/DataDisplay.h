#ifndef _DATA_DISPLAY_H_
#define _DATA_DISPLAY_H_

#include "Arduino.h"

class DataDisplay
{
private:
    uint8_t x;
    uint8_t y;
    uint8_t size;
    String text;

public:
    DataDisplay(/* args */);
    DataDisplay(uint8_t, uint8_t, uint8_t);
    ~DataDisplay();

    uint8_t getX();
    uint8_t getY();
    uint8_t getSize();
    String getText();

    void setX(uint8_t);
    void setY(uint8_t);
    void setSize(uint8_t);
    void setText(String);

    void (*refresh)();
};

#endif // _DATA_DISPLAY_H_