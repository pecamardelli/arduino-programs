#include "header.h"

void parser(char *command)
{
    // Tokenizing arguments
    char *args[MAX_COMMAND_ARGS];
    clearArgs(args);

    // Using space character as delimiter
    const char *delimiter = " ";

    char *token = strtok(command, delimiter);
    byte index = 0;

    while (token != NULL)
    {
        args[index] = token;
        index++;
        token = strtok(NULL, delimiter);
    }

    if (strncmp(args[0], "help", 4) == 0)
    { /*printHelp();*/
    }
    else if (strncmp(args[0], "set", 3) == 0)
    {
        if (strncmp(args[1], "help", 4) == 0 or !strlen(args[1]))
        { /*setCommandHelp();*/
        }
        else if (strncmp(args[1], "hostname", 8) == 0)
            sys.setHostname(args[2]); // By default, args[2] is an empty string. setHostname will deal with it.
        else if (strncmp(args[1], "ip", 2) == 0)
            sys.setIpAddress(args[2], IP_ADDRESS);
        else if (strncmp(args[1], "mask", 2) == 0)
            sys.setIpAddress(args[2], SUBNET_MASK);
        else if (strncmp(args[1], "gateway", 7) == 0)
            sys.setIpAddress(args[2], DEFAULT_GATEWAY);
        else if (strncmp(args[1], "dns", 3) == 0)
            sys.setIpAddress(args[2], DNS_SERVER);
        else if (strncmp(args[1], "mac", 3) == 0)
            sys.printData(F("Not implemented..."), true);
        else if (strncmp(args[1], "datetime", 3) == 0)
            clock.setDateTime(args[2], args[3]);
        else
        {
            sys.printData(F("Unknown subcommand \""), false);
            sys.printData(args[1], false);
            sys.printData(F("\""), true);
        }
    }
    else if (strncmp(args[0], "relay", 5) == 0)
    {
        // In some cases, args[1] can be a string. We'll evaluate this first.
        if (strncmp(args[1], "help", 4) == 0)
        { /*return relayCommandHelp();*/
        }
        if (strncmp(args[1], "info", 4) == 0)
        { /*return getRelayInfo();*/
        }

        // Now check if args[1] is a valid pin number.
        const byte pin = (byte)args[1];

        if (strncmp(args[2], "create", 6) == 0)
            createRelay(pin);
        else if (strncmp(args[2], "delete", 6) == 0)
            deleteRelay(pin);
        else if (strncmp(args[2], "enable", 6) == 0)
            setRelayStatus(pin, true);
        else if (strncmp(args[2], "disable", 7) == 0)
            setRelayStatus(pin, false);
        else if (strncmp(args[2], "resume", 6) == 0)
            resumeRelay(pin);
        else if (strncmp(args[2], "desc", 4) == 0)
            setRelayDesc(pin, args[3]);
        else if (strncmp(args[2], "starth", 6) == 0)
            setRelayParams(pin, args[3], TYPE_START_HOUR);
        else if (strncmp(args[2], "startm", 6) == 0)
            setRelayParams(pin, args[3], TYPE_START_MIN);
        else if (strncmp(args[2], "endh", 4) == 0)
            setRelayParams(pin, args[3], TYPE_END_HOUR);
        else if (strncmp(args[2], "endm", 4) == 0)
            setRelayParams(pin, args[3], TYPE_END_MIN);
        else if (strncmp(args[2], "pin", 3) == 0)
            changeRelayPin(pin, args[3]);
        else if (strncmp(args[2], "on", 2) == 0)
            switchRelay(pin, LOW, true);
        else if (strncmp(args[2], "off", 3) == 0)
            switchRelay(pin, HIGH, true);
        else
        {
            sys.printData(F("Unknown subcommand \""), false);
            sys.printData(args[2], false);
            sys.printData(F("\""), true);
        }
    }
    else if (strncmp(args[0], "show", 4) == 0)
    {
        if (strncmp(args[1], "help", 4) == 0 or !strlen(args[1]))
        { /*showCommandHelp();*/
        }
        else if (strncmp(args[1], "relays", 6) == 0)
            getRelayInfo();
        else if (strncmp(args[1], "ip", 2) == 0)
        { /*printIpAddress();*/
        }
        else if (strncmp(args[1], "version", 7) == 0)
        {
            sys.printData(F("ARDUINO RELAY TIMER Version "), false);
            sys.printData(VERSION, true);
        }
        else
        {
            sys.printData(F("Unknown subcommand \""), false);
            sys.printData(args[1], false);
            sys.printData(F("\""), true);
        }
    }
    else if (strncmp(args[0], "hostname", 8) == 0)
        sys.printData(sys.config.hostname, true);
    else if (strncmp(args[0], "time", 4) == 0)
        sys.printData(clock.getDate(), true);
    else if (strncmp(args[0], "save", 4) == 0)
        sys.saveData();
    else if (strncmp(args[0], "reboot", 6) == 0)
        digitalWrite(sys.config.resetPin, LOW);
    else if (strncmp(args[0], "exit", 4) == 0 ||
             strncmp(args[0], "quit", 4) == 0 &&
                 sys.output == COMM_TELNET)
    {
        network.closeConnection();
    }
    else if (strncmp(args[0], "defragEeprom", 12) == 0)
        sys.printData(F("Feature not implemented"), true);
    else
    {
        sys.printData(F("Bad command \""), false);
        sys.printData(args[0], false);
        sys.printData(F("\""), true);
    }
}
