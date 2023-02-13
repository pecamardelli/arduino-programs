
#include <Ethernet.h>

// ----------- MACROS ------------- //
#ifndef MACROS_H
#define MACROS_H

#define MAX_COMMAND_LEN 64

#endif

#ifndef SYSTEM_H
#define SYSTEM_H

#define VERSION F("2.0")
#define BAUD_RATE 9600

class System
{
    bool charAllowed(char c);
    byte specialChars[6];

public:
    System();
    String hostname;
    byte macAddress[6];
    IPAddress ipAddress;
    IPAddress subnetMask;
    IPAddress gateway;
    IPAddress dnsServer;

    void System::getSerialInput();
};

#endif
