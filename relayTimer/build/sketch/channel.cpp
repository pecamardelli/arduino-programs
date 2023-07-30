#line 1 "/home/pablin/arduino_programs/relayTimer/channel.cpp"
#include "header.h"

extern NodeList nodes;

template <class T>
Channel<T>::Channel(/* args */)
{
    // Array of special chars allowed on inputs
    specialChars[0] = 0x20;
    specialChars[1] = 0x2d;
    specialChars[2] = 0x2e;
    specialChars[3] = 0x2f;
    specialChars[4] = 0x3a;
}

template <class T>
Channel<T>::~Channel()
{
}

template <class T>
String Channel<T>::getInput()
{
    if (client->available() <= 0)
        return;

    char c;
    String input;

    while (client->available() > 0)
    {
        c = client->read();
        // Serial.print(String(int(c)) + " ");
        //  Include letters, digits, and other allowed chars. Add more allowed characters
        //  at the definition of the specialChars array.
        if (isalpha(c) || isdigit(c) || charAllowed(c))
            input.concat(c);
        else
            input.concat((char)0x20);

        delay(5);
    }

    return input;
};

template <class T>
bool Channel<T>::charAllowed(char c)
{
    for (byte i = 0; i < sizeof(specialChars) / sizeof(byte); i++)
    {
        if ((int)c == specialChars[i])
            return true;
    }
    return false;
};

template class Channel<EthernetClient *>;
template class Channel<Stream *>;
