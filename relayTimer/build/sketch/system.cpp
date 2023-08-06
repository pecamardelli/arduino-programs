#line 1 "/home/pablin/arduino_programs/relayTimer/system.cpp"
#include "header.h"

extern NodeList nodes;

System::System()
{
  configChanged = false;

  loadSystemData();
};

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char *sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif // __arm__

int System::getFreeMemory()
{
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char *>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif // __arm__
}

String System::setHostname(String newHostname)
{
  if (newHostname.length() == 0)
    return F("Error: hostname cannot be empty.");

  if (newHostname.length() > MAX_HOSTNAME_LEN)
    return F("Error: hostname exceedes maximum.");

  // Looks like we're good...
  newHostname.toCharArray(config.hostname, MAX_HOSTNAME_LEN);

  saveSystemData();

  return newHostname;
};

String System::setIpAddress(String newIpAddress)
{
  if (!config.ethernetConfig.ipAddress.fromString(newIpAddress))
    return "Error invalid ip address: " + newIpAddress;

  Ethernet.setLocalIP(config.ethernetConfig.ipAddress);

  saveSystemData();

  return newIpAddress;
};

String System::setSubnetMask(String newSubnetMask)
{
  if (!config.ethernetConfig.subnetMask.fromString(newSubnetMask))
    return F("Error: invalid subnet mask.");

  Ethernet.setSubnetMask(config.ethernetConfig.subnetMask);

  saveSystemData();

  return newSubnetMask;
};

String System::setDefaultGateway(String newDefaultGateway)
{
  if (!config.ethernetConfig.gateway.fromString(newDefaultGateway))
    return F("Error: invalid default gateway.");

  Ethernet.setGatewayIP(config.ethernetConfig.gateway);

  saveSystemData();

  return newDefaultGateway;
};

String System::setDnsServer(String newDnsServer)
{
  if (!config.ethernetConfig.dnsServer.fromString(newDnsServer))
    return F("Error: invalid DNS server.");

  Ethernet.setDnsServerIP(config.ethernetConfig.dnsServer);

  saveSystemData();

  return newDnsServer;
};

String System::exec(Command *com)
{
  String output = "";
  // ----- Do some debugging ----- //

  // for (uint8_t i = 0; i < com->count; i++)
  // {
  //     output += "Argument " + String(i) + ": ");
  //     output += com->args[i]);
  // }
  // output += "Total args: " + String(com->count));

  // ---------------------------- //

  // output += "Free memory before: " + String(getFreeMemory()));

  if (com->args[0].equals("set"))
  {
    if (com->args[1].equals("date"))
      output = clock.setDateTime(com->args[2], com->args[3]);
  }
  else if (com->args[0].equals("relay"))
  {
    if (com->args[1].equals("info") || com->args[1].equals("status"))
    {
      return nodes.getRelayInfo();
    }

    const uint8_t pin = com->args[1].toInt();

    if (com->args[2].equals("create"))
    {
      Relay *newRelay = nodes.createRelay(pin);
      if (newRelay == NULL)
        return F("Error: pin not available");
      else
        nodes.addNode(newRelay);
    }

    Relay *relay = nodes.searchByPin(pin);
    if (relay == NULL)
    {
      output = F("No relay defined on pin ");
      output += pin;
      return output;
    }

    if (com->args[2].equals("enable"))
    {
      output = relay->setStatus(enabled);
    }
    else if (com->args[2].equals("disable"))
    {
      output = relay->setStatus(disabled);
    }
    else if (com->args[2].equals("starth"))
    {
      output = relay->setStartHour(com->args[3]);
    }
    else if (com->args[2].equals("startm"))
    {
      output = relay->setStartMinute(com->args[3]);
    }
    else if (com->args[2].equals("endh"))
    {
      output = relay->setEndHour(com->args[3]);
    }
    else if (com->args[2].equals("endm"))
    {
      output = relay->setEndMinute(com->args[3]);
    }
  }
  else if (com->args[0].equals("freemem"))
  {
    output = String(getFreeMemory());
  }
  else if (com->args[0].equals("hostname"))
    output = config.hostname;
  else if (com->args[0].equals("sysinfo"))
  {
    output = String(F("Hostname:\t")) + String(config.hostname) + "\n";
    output += String(F("IP Address:\t")) + ipToString(Ethernet.localIP()) + "\n";
    output += String(F("Subnet mask:\t")) + ipToString(Ethernet.subnetMask()) + "\n";
    output += String(F("Gateway:\t")) + ipToString(Ethernet.gatewayIP()) + "\n";
    output += String(F("DNS Server:\t")) + ipToString(Ethernet.dnsServerIP()) + "\n";
    output += String(F("Free memory:\t")) + getFreeMemory() + " bytes\n";
    output += String(F("EEPROM length:\t")) + EEPROM.length() + " bytes\n";
    output += String(F("Relays defined:\t")) + nodes.getNodeNumber() + "\n";
    output += String(F("System date:\t")) + clock.getDate() + "\n";
  }
  else if (com->args[0].equals("reset"))
  {
    if (com->args[1].equals("config"))
    {
      output = resetConfig();
    }
    else if (com->args[1].equals("relays"))
    {
      nodes.eraseRelaysFromEEPROM();
    }
  }
  else if (com->args[0].equals("exit") || com->args[0].equals("quit"))
  {
    telnet.closeConnection();
  }

  return output;
};

void System::loadSystemData()
{
  // LOAD SYSTEM CONFIGURATIONS
  // They're stored at the end of the EEPROM.
  // Relay data is stored at the beginning.

  eeAddress = EEPROM.length() - sizeof(system_t) - 1;
  EEPROM.get(eeAddress, config);

  if (config.hostname[0] == 0xff)
    resetConfig();
};

void System::saveSystemData()
{
  eeAddress = EEPROM.length() - sizeof(config) - 1;
  EEPROM.put(eeAddress, config);
}

String System::ipToString(IPAddress address)
{
  String stringifiedAddress;
  stringifiedAddress = String(address[0]) + ".";
  stringifiedAddress += String(address[1]) + ".";
  stringifiedAddress += String(address[2]) + ".";
  stringifiedAddress += String(address[3]);

  return stringifiedAddress;
}

String System::resetConfig()
{
  strcpy(config.hostname, "arduino");
  config.ethernetConfig.ipAddress.fromString("192.168.40.8");
  Ethernet.setLocalIP(config.ethernetConfig.ipAddress);

  config.ethernetConfig.subnetMask.fromString("255.255.255.0");
  Ethernet.setSubnetMask(config.ethernetConfig.subnetMask);

  config.ethernetConfig.gateway.fromString("192.168.40.1");
  Ethernet.setGatewayIP(config.ethernetConfig.gateway);

  config.ethernetConfig.dnsServer.fromString("179.42.171.21");
  Ethernet.setDnsServerIP(config.ethernetConfig.dnsServer);

  config.ethernetConfig.macAddress[0] = 0xDE;
  config.ethernetConfig.macAddress[1] = 0xAD;
  config.ethernetConfig.macAddress[2] = 0xBE;
  config.ethernetConfig.macAddress[3] = 0xEF;
  config.ethernetConfig.macAddress[4] = 0xF0;
  config.ethernetConfig.macAddress[5] = 0x18;
  Ethernet.setMACAddress(config.ethernetConfig.macAddress);

  config.relayCheckInterval = 1000;

  configChanged = true;

  saveSystemData();

  return F("Configuration reset to defaults.");
}
