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
    else if (strncmp(com->args[0], "set", 3) == 0)
    {
        if (strncmp(com->args[1], "help", 4) == 0 or !strlen(com->args[1]))
        { /*setCommandHelp();*/
        }
        else if (strncmp(com->args[1], "hostname", 8) == 0)
            sys.setHostname(com->args[2]); // By default, args[2] is an empty string. setHostname will deal with it.
        // else if (strncmp(com->args[1], "ip", 2) == 0)
        //     sys.setIpAddress(com->args[2], IP_ADDRESS);
        // else if (strncmp(com->args[1], "mask", 2) == 0)
        //     sys.setIpAddress(com->args[2], SUBNET_MASK);
        // else if (strncmp(com->args[1], "gateway", 7) == 0)
        //     sys.setIpAddress(com->args[2], DEFAULT_GATEWAY);
        // else if (strncmp(com->args[1], "dns", 3) == 0)
        //     sys.setIpAddress(com->args[2], DNS_SERVER);
        // else if (strncmp(com->args[1], "mac", 3) == 0)
        //     sys.printData(F("Not implemented..."), true);
        // else if (strncmp(com->args[1], "datetime", 3) == 0)
        //     clock.setDateTime(com->args[2], com->args[3]);
        else
        {
            client.print(F("Unknown subcommand \""));
            client.print(com->args[1]);
            client.println(F("\""));
        }
    }
    else if (strncmp(com->args[0], "freemem", 7) == 0)
    {
        client.println("Free memory: " + String(sys.getFreeMemory()) + " bytes.");
    }
    else if (strncmp(com->args[0], "hostname", 8) == 0)
        client.println(sys.hostname);
    else if (strncmp(com->args[0], "exit", 4) == 0 || strncmp(com->args[0], "quit", 4) == 0)
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
