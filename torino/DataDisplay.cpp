#include "DataDisplay.h"

DataDisplay::DataDisplay(/* args */)
{
    setX(0);
    setY(0);
    setSize(1);
    setText("");
}

DataDisplay::DataDisplay(uint8_t x, uint8_t y, uint8_t size)
{
    setX(x);
    setY(y);
    setSize(size);
    setText("");
}

DataDisplay::~DataDisplay()
{
}

/**************************************************************************/
/*!
    @brief  Returns the value of X.
    @return The value of X.
*/
/**************************************************************************/
uint8_t DataDisplay::getX()
{
    return x;
}

/**************************************************************************/
/*!
    @brief  Returns the value of Y.
    @return The value of Y.
*/
/**************************************************************************/
uint8_t DataDisplay::getY()
{
    return y;
}

/**************************************************************************/
/*!
    @brief  Returns the text size.
    @return The text size.
*/
/**************************************************************************/
uint8_t DataDisplay::getSize()
{
    return size;
}

/**************************************************************************/
/*!
    @brief  Returns the text to be displayed.
    @return The text.
*/
/**************************************************************************/
String DataDisplay::getText()
{
    return text;
}

/**************************************************************************/
/*!
    @brief  Sets the value for X.
    @param  _x The new value for X.
    @return nothing
*/
/**************************************************************************/
void DataDisplay::setX(uint8_t _x)
{
    x = _x;
}

/**************************************************************************/
/*!
    @brief  Sets the value for Y.
    @param  _x The new value for Y.
    @return nothing
*/
/**************************************************************************/
void DataDisplay::setY(uint8_t _y)
{
    y = _y;
}

/**************************************************************************/
/*!
    @brief  Sets the value for the size.
    @param  _size The new value for the size.
    @return nothing
*/
/**************************************************************************/
void DataDisplay::setSize(uint8_t _size)
{
    size = _size;
}

/**************************************************************************/
/*!
    @brief  Sets the text to be displayed.
    @param  _text The new text.
    @return nothing
*/
/**************************************************************************/
void DataDisplay::setText(String _text)
{
    text = _text;
}