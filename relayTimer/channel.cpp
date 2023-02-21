#include "header.h"

extern node_t *first;
extern node_t *last;

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

    if (client->available() <= 0)
        return;

    // Looks like we have a command to parse. Let's do it.
    char *input = (char *)calloc(MAX_COMMAND_LEN, sizeof(char));

    while (client->available() > 0)
    {
        c = client->read();
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
        delay(5);
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

template <class T>
char *Channel<T>::exec(Command *com)
{

    // ----- Do some debugging ----- //

    // for (uint8_t i = 0; i < com->count; i++)
    // {
    //     client->print("Argument " + String(i) + ": ");
    //     client->println(com->args[i]);
    // }
    // client->println("Total args: " + String(com->count));

    // ---------------------------- //

    // client->println("Free memory before: " + String(sys.getFreeMemory()));

    if (strncmp(com->args[0], "help", 4) == 0)
    {
        client->println(F("time\t\t\t--> Returns the current time."));
        client->println(F("relay (pinNumber)\t--> Runs command on specified relay. Use 'relay help' for further options."));
        client->println(F("set (parameter)\t\t--> Sets a given parameter. Use 'set help' for further options."));
        client->println(F("show (parameter)\t--> Shows specified information. Use 'show help' for further options."));
        client->println(F("save\t\t\t--> Save changes."));
        client->println(F("exit\t\t\t--> Close connection (telnet only)."));
        client->println(F("quit\t\t\t--> Close connection (telnet only)."));
    }
    else if (strncmp(com->args[0], "set", 3) == 0)
    {
        if (strncmp(com->args[1], "help", 4) == 0 or !strlen(com->args[1]))
        {
            client->println(F("hostname\t\t--> Sets the system name."));
            client->println(F("ip\t\t--> Sets system's ip address."));
            client->println(F("mask\t\t--> Sets system's subnet mask."));
            client->println(F("gateway\t\t--> Sets system's default gateway."));
            client->println(F("dns\t\t--> Sets system's dns server."));
            client->println(F("mac\t\t--> Sets system's mac address."));
        }
        else if (strncmp(com->args[1], "hostname", 8) == 0)
            client->println(sys.setHostname(com->args[2])); // By default, args[2] is an empty string. setHostname will deal with it.
        else if (strncmp(com->args[1], "ip", 2) == 0)
            client->println(sys.setIpAddress(com->args[2]));
        else if (strncmp(com->args[1], "mask", 2) == 0)
            client->println(sys.setSubnetMask(com->args[2]));
        else if (strncmp(com->args[1], "gateway", 2) == 0)
            client->println(sys.setDefaultGateway(com->args[2]));
        else if (strncmp(com->args[1], "dns", 2) == 0)
            client->println(sys.setDnsServer(com->args[2]));
        else if (strncmp(com->args[1], "mac", 2) == 0)
            client->println("Not implemented");

        else if (strncmp(com->args[0], "relay", 5) == 0)
        {
            client->print(F("Unknown subcommand \""));
            client->print(com->args[1]);
            client->println(F("\""));
        }
    }
    else if (strncmp(com->args[0], "relay", 5) == 0)
    {
        if (strncmp(com->args[1], "help", 4) == 0 or !strlen(com->args[1]))
        {
            client->println(F("relay info\t\t\t-> Shows relay information."));
            client->println(F("relay (pinNumber) create\t\t-> Creates a relay at the specified pin."));
            client->println(F("relay (pinNumber) delete \t-> Deletes the specified relay."));
            client->println(F("relay (pinNumber) enable\t-> Enables the specified relay."));
            client->println(F("relay (pinNumber) disable\t-> Disables the specified relay."));
            client->println(F("relay (pinNumber) resume\t-> Resumes operation of the specified relay."));
            client->println(F("relay (pinNumber) desc (string)\t-> Adds a description to the specified relay."));
            client->println(F("relay (pinNumber) starth (hour)\t-> Sets starting hour."));
            client->println(F("relay (pinNumber) endh (hour)\t-> Sets the ending hour."));
            client->println(F("relay (pinNumber) startm (minute)\t-> Sets the starting minute."));
            client->println(F("relay (pinNumber) endm (minute)\t-> Sets the ending minute."));
            client->println(F("relay (pinNumber) pin (pinNumber)\t-> Sets a new pin for the specified relay."));
            client->println(F("relay (pinNumber) on (pinNumber)\t-> Turns on the specified relay."));
            client->println(F("relay (pinNumber) off (pinNumber)\t-> Turns off the specified relay."));
            return;
        }
        else if (strncmp(com->args[1], "info", 4) == 0)
        {
            getRelayInfo();
            return;
        }

        const uint8_t pin = atoi(com->args[1]);
        // client->println("Pin: " + String(pin));

        if (strncmp(com->args[2], "create", 6) == 0)
        {
            client->println(sys.createRelay(pin));
        }
        else if (strncmp(com->args[2], "on", 2) == 0)
        {
            Relay *relay = sys.searchByPin(pin);
            if (relay == NULL)
            {
                client->println("No relay defined on pin " + String(pin));
                return;
            }

            relay->setMode(LOW);
        }
        else if (strncmp(com->args[2], "off", 3) == 0)
        {
            Relay *relay = sys.searchByPin(pin);
            if (relay == NULL)
            {
                client->println("No relay defined on pin " + String(pin));
                return;
            }

            relay->setMode(HIGH);
        }
        else
        {
            client->print(F("Unknown subcommand: "));
            client->println(com->args[1]);
        }
    }
    else if (strncmp(com->args[0], "freemem", 7) == 0)
    {
        client->println("Free memory: " + String(sys.getFreeMemory()) + " bytes.");
    }
    else if (strncmp(com->args[0], "hostname", 8) == 0)
        client->println(sys.hostname);
    else if (strncmp(com->args[0], "exit", 4) == 0 || strncmp(com->args[0], "quit", 4) == 0)
    {
        telnet.closeConnection();
    }
    else
    {
        client->println("Bad command: " + String(com->args[0]));
    }

    // client->println("Free memory after: " + String(sys.getFreeMemory()));
};

template <class T>
void Channel<T>::getRelayInfo()
{
    client->println(F("PIN\tENABLED\tDESCRIPTION\t\tSTART\tEND\tSTATUS\tUPTIME"));
    node_t *aux = first;

    while (aux != NULL)
    {
        client->print(String(aux->relay->pin) + "\t");

        const char *_enabled = (aux->relay->enabled) ? "true" : "false";
        client->print(_enabled);
        client->print("\t");
        client->print(aux->relay->desc);

        const uint8_t *descLen = strlen(aux->relay->desc);

        if (descLen < 8)
            client->print("\t\t\t");
        else if (descLen < 16)
            client->print("\t\t");
        else
            client->print("\t");

        String starth = (aux->relay->startHour < 10) ? "0" + String(aux->relay->startHour) : String(aux->relay->startHour);
        String startm = (aux->relay->startMin < 10) ? "0" + String(aux->relay->startMin) : String(aux->relay->startMin);
        client->print(starth + ":" + startm + "\t");

        String endh = (aux->relay->endHour < 10) ? "0" + String(aux->relay->endHour) : String(aux->relay->endHour);
        String endm = (aux->relay->endMin < 10) ? "0" + String(aux->relay->endMin) : String(aux->relay->endMin);
        client->print(endh + ":" + endm + "\t");

        const char *_mode = digitalRead(aux->relay->pin) ? "off" : "on";
        client->print(_mode);
        client->print("\t");

        if (aux->relay->startTime)
        {
        }
        // client->print(parseUpTime(aux->relay->startTime));
        else
            client->print(F("--d --:--:--"));

        client->print("\t");

        if (aux->changeFlag)
            client->println("*");
        else
            client->println("");
        aux = aux->next;
    }
}

template class Channel<EthernetClient *>;
template class Channel<Stream *>;
