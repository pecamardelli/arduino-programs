#include "header.h"

System sys;
Parser parser;
EthernetServer server(23); // Telnet listens on port 23
Telnet telnet;
SerialChannel serial;
NodeList nodes;
// Clock clock;

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

  Ethernet.begin(
      sys.config.ethernetConfig.macAddress,
      sys.config.ethernetConfig.ipAddress,
      sys.config.ethernetConfig.dnsServer,
      sys.config.ethernetConfig.gateway,
      sys.config.ethernetConfig.subnetMask);

  server.begin();
}

void loop()
{
  telnet.checkAvailable();
  serial.checkAvailable();

  nodes.checkRelays();
}
