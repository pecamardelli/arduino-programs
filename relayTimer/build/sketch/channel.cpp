#line 1 "/home/pablin/arduino_programs/relayTimer/channel.cpp"
#include "header.h"

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
char *Channel<T>::getInput()
{
    uint8_t charIndex = 0;
    char c;

    if (client.available() <= 0)
        return;

    // Looks like we have a command to parse. Let's do it.
    char *input = (char *)calloc(MAX_COMMAND_LEN, sizeof(char));

    while (client.available() > 0)
    {
        c = client.read();
        // Serial.print(String(int(c)) + " ");
        //  Include letters, digits, and other allowed chars. Add more allowed characters
        //  at the definition of the specialChars array.
        if (isalpha(c) or isdigit(c) or charAllowed(c))
        {
            input[charIndex++] = (char)c;
        }
        else
        {
            input[charIndex++] = (char)0x20;
        }
    }

    return input;
};

template <class T>
bool Channel<T>::charAllowed(char c)
{
    for (byte i = 0; i < sizeof(specialChars) / sizeof(byte); i++)
    {
        if (c == specialChars[i])
            return true;
    }
    return false;
};

template class Channel<EthernetClient>;
template class Channel<Serial>;
