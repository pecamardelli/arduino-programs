
#include "header.h"

System sys;
Parser parser;
EthernetServer server(23); // Telnet listens on port 23

void setup()
{
    Serial.begin(9600);
    Ethernet.begin(sys.macAddress, sys.ipAddress, sys.dnsServer, sys.gateway, sys.subnetMask);
    server.begin();
}

void loop()
{
    if (Serial.available())
    {
        Serial.println("Free memory before: " + String(sys.getFreeMemory()));
        char *input = sys.getSerialInput();
        parser.parse(input);
        free(input);
        Serial.println("Free memory after: " + String(sys.getFreeMemory()));
    }
}
