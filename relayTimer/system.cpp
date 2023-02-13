#include "header.h"

System::System()
{
    hostname = "Arduino Relay Timer";
    ipAddress.fromString("192.168.40.3");
    subnetMask.fromString("255.255.255.0");
    gateway.fromString("192.168.40.1");
    dnsServer.fromString("179.42.171.21");

    macAddress[0] = 0xDE;
    macAddress[1] = 0xAD;
    macAddress[2] = 0xBE;
    macAddress[3] = 0xEF;
    macAddress[4] = 0xF0;
    macAddress[5] = 0x18;

    // Array of special chars allowed on inputs
    specialChars[0] = 0x20;
    specialChars[1] = 0x2d;
    specialChars[2] = 0x2e;
    specialChars[3] = 0x2f;
    specialChars[4] = 0x3a;
};

void System::getSerialInput()
{
    char c;
    uint8_t charsReceived = 0;
    char buffer[MAX_COMMAND_LEN] = {""};

    while (Serial.available() > 0)
    {
        c = (char)Serial.read();

        // Include letters, digits, and other allowed chars. Add more allowed characters
        // at the definition of the specialChars array.
        if (isalpha(c) or isdigit(c) or charAllowed(c))
        {
            buffer[charsReceived] = (char)c;
            charsReceived++;
            if (charsReceived >= MAX_COMMAND_LEN - 1)
                break;
        }
        delay(5);
    }

    buffer[charsReceived] = (char)0x20;

    if (charsReceived)
        Serial.println(buffer);
};

bool System::charAllowed(char c)
{
    for (byte i = 0; i < sizeof(specialChars) / sizeof(byte); i++)
    {
        if (c == specialChars[i])
            return true;
    }
    return false;
}
