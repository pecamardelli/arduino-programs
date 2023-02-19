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
char *Channel<T>::exec(Command *com)
{

    // ----- Do some debugging ----- //

    // for (uint8_t i = 0; i < com->count; i++)
    // {
    //     client.print("Argument " + String(i) + ": ");
    //     client.println(com->args[i]);
    // }
    // client.println("Total args: " + String(com->count));

    // ---------------------------- //

    // client.println("Free memory before: " + String(sys.getFreeMemory()));

    if (strncmp(com->args[0], "help", 4) == 0)
    {
        client.println(F("time\t\t\t--> Returns the current time."));
        client.println(F("relay (pinNumber)\t--> Runs command on specified relay. Use 'relay help' for further options."));
        client.println(F("set (parameter)\t\t--> Sets a given parameter. Use 'set help' for further options."));
        client.println(F("show (parameter)\t--> Shows specified information. Use 'show help' for further options."));
        client.println(F("save\t\t\t--> Save changes."));
        client.println(F("reboot\t\t\t--> Restarts the system (TODO)."));
        client.println(F("exit\t\t\t--> Close connection (telnet only)."));
        client.println(F("quit\t\t\t--> Close connection (telnet only)."));
    }
    else if (strncmp(com->args[0], "freemem", 7) == 0)
    {
        client.println("Free memory: " + String(sys.getFreeMemory()));
    }
    else if (strncmp(com->args[0], "exit", 4) == 0)
    {
        telnet.closeConnection();
    }
    else
    {
        client.println("Bad command: " + String(com->args[0]));
    }

    // client.println("Free memory after: " + String(sys.getFreeMemory()));
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
