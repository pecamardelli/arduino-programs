#include "header.h"

System sys;
Parser parser;
EthernetServer server(23); // Telnet listens on port 23
Telnet telnet;
SerialChannel serial;

node_t *first = NULL;
node_t *last = NULL;

void setup()
{
  Serial.begin(9600);

  // Check for Ethernet hardware present
  // if (Ethernet.hardwareStatus() == EthernetNoHardware)
  // {
  //     Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
  // }
  // if (Ethernet.linkStatus() == LinkOFF)
  // {
  //     Serial.println("Ethernet cable is not connected.");
  // }
  Ethernet.begin(sys.macAddress, sys.ipAddress, sys.dnsServer, sys.gateway, sys.subnetMask);
  server.begin();
}

void loop()
{
  telnet.checkAvailable();
  serial.checkAvailable();
}
