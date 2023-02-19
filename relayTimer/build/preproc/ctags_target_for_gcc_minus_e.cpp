# 1 "/home/pablin/arduino_programs/relayTimer/relayTimer.ino"
# 2 "/home/pablin/arduino_programs/relayTimer/relayTimer.ino" 2

System sys;
Parser parser;
EthernetServer server(23); // Telnet listens on port 23
Telnet telnet;

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
  telnet.checkConnection();

  if (Serial.available())
  {
    Serial.println("Free memory before: " + String(sys.getFreeMemory()));
    char *input = sys.getSerialInput();
    Command *com = parser.parse(input);

    if (strncmp(com->args[0], "help", 4) == 0)
    {
      Serial.println("Help wanted...");
    }
    else if (strncmp(com->args[0], "exit", 4) == 0)
    {
      // telnet.closeConnection();
    }
    else
    {
      Serial.println("Bad command: " + String(com->args[0]));
    }

    com->freeMem();
    delete com;
    Serial.println("Free memory after: " + String(sys.getFreeMemory()));
  }
}
