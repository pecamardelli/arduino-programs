#include "header.h"

extern NodeList nodes;

System::System()
{
  configChanged = false;
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

  if (com->args[0].equals("help"))
  {
    output += F("time\t\t\t--> Returns the current time.\n");
    output += F("relay (pinNumber)\t--> Runs command on specified relay. Use 'relay help' for further options.\n");
    output += F("set (parameter)\t\t--> Sets a given parameter. Use 'set help' for further options.\n");
    output += F("show (parameter)\t--> Shows specified information. Use 'show help' for further options.\n");
    output += F("save\t\t\t--> Save changes.\n");
    output += F("exit\t\t\t--> Close connection (telnet only).\n");
    output += F("quit\t\t\t--> Close connection (telnet only).\n");
  }
  else if (com->args[0].equals("set"))
  {
    if (com->args[1].equals("help") || com->args[1].length() == 0)
    {
      output += F("hostname\t--> Sets the system name.\n");
      output += F("ip\t\t--> Sets system's ip address.\n");
      output += F("mask\t\t--> Sets system's subnet mask.\n");
      output += F("gateway\t\t--> Sets system's default gateway.\n");
      output += F("dns\t\t--> Sets system's dns server.\n");
      output += F("mac\t\t--> Sets system's mac address.\n");
    }
    else if (com->args[1].equals("hostname"))
      output = setHostname(com->args[2]); // By default, args[2] is an empty string. setHostname will deal with it.
    else if (com->args[1].equals("ip"))
      output = setIpAddress(com->args[2]);
    else if (com->args[1].equals("mask"))
      output = setSubnetMask(com->args[2]);
    else if (com->args[1].equals("gateway"))
      output = setDefaultGateway(com->args[2]);
    else if (com->args[1].equals("dns"))
      output = setDnsServer(com->args[2]);
    else if (com->args[1].equals("mac"))
      output = F("Not implemented");
    else if (com->args[1].equals("date"))
      output = clock.setDateTime(com->args[2], com->args[3]);
    else
    {
      output += F("Unknown subcommand \"");
      output += com->args[1];
      output += F("\"");
    }
  }
  else if (com->args[0].equals("relay"))
  {
    if (com->args[1].equals("help") || com->args[1].length() == 0)
    {
      output += F("relay info\t\t\t\t-> Shows this information.\n");
      output += F("relay (pinNumber) create\t\t-> Creates a relay at the specified pin.\n");
      output += F("relay (pinNumber) delete \t\t-> Deletes the specified relay.\n");
      output += F("relay (pinNumber) enable\t\t-> Enables the specified relay.\n");
      output += F("relay (pinNumber) disable\t\t-> Disables the specified relay.\n");
      output += F("relay (pinNumber) resume\t\t-> Resumes operation of the specified relay.\n");
      output += F("relay (pinNumber) desc (string)\t\t-> Adds a description to the specified relay.\n");
      output += F("relay (pinNumber) starth (hour)\t\t-> Sets starting hour.\n");
      output += F("relay (pinNumber) endh (hour)\t\t-> Sets the ending hour.\n");
      output += F("relay (pinNumber) startm (minute)\t-> Sets the starting minute.\n");
      output += F("relay (pinNumber) endm (minute)\t\t-> Sets the ending minute.\n");
      output += F("relay (pinNumber) pin (pinNumber)\t-> Sets a new pin for the specified relay.\n");
      output += F("relay (pinNumber) on (pinNumber)\t-> Turns on the specified relay.\n");
      output += F("relay (pinNumber) off (pinNumber)\t-> Turns off the specified relay.\n");
      return output;
    }
    else if (com->args[1].equals("info") || com->args[1].equals("status"))
    {
      output = nodes.getRelayInfo();
      return output;
    }

    const uint8_t pin = com->args[1].toInt();

    uint8_t index = nodes.searchByPin(pin);
    if (index == NULL)
    {
      output = F("No relay defined on pin ");
      output += pin;
      return output;
    }

    if (com->args[2].equals("on"))
    {
      output = nodes.relayArray[index].switchOn();
    }
    else if (com->args[2].equals("off"))
    {
      output = nodes.relayArray[index].switchOff();
    }
    else if (com->args[2].equals("description"))
    {
      output = nodes.relayArray[index].setDesc(com->args[3]);
    }
    else if (com->args[2].equals("enable"))
    {
      output = nodes.relayArray[index].setStatus(enabled);
    }
    else if (com->args[2].equals("disable"))
    {
      output = nodes.relayArray[index].setStatus(disabled);
    }
    else if (com->args[2].equals("starth"))
    {
      output = nodes.relayArray[index].setStartHour(com->args[3]);
    }
    else if (com->args[2].equals("startm"))
    {
      output = nodes.relayArray[index].setStartMinute(com->args[3]);
    }
    else if (com->args[2].equals("endh"))
    {
      output = nodes.relayArray[index].setEndHour(com->args[3]);
    }
    else if (com->args[2].equals("endm"))
    {
      output = nodes.relayArray[index].setEndMinute(com->args[3]);
    }
    else
    {
      output += F("Unknown subcommand: ");
      output += com->args[1];
      output += "\n";
    }
  }
  else if (com->args[0].equals("freemem"))
  {
    output = F("Free memory: ");
    output += String(getFreeMemory());
    output += F(" bytes.");
  }
  else if (com->args[0].equals("hostname"))
    output = config.hostname;
  else if (com->args[0].equals("sysinfo"))
  {
    output = String(F("Board type:\tArduino Uno\n"));
    output += String(F("Hostname:\t")) + String(config.hostname) + "\n";
    output += String(F("IP Address:\t")) + ipToString(Ethernet.localIP()) + "\n";
    output += String(F("Subnet mask:\t")) + ipToString(Ethernet.subnetMask()) + "\n";
    output += String(F("Gateway:\t")) + ipToString(Ethernet.gatewayIP()) + "\n";
    output += String(F("DNS Server:\t")) + ipToString(Ethernet.dnsServerIP()) + "\n";
    output += String(F("Free memory:\t")) + getFreeMemory() + " bytes\n";
    output += String(F("EEPROM length:\t")) + EEPROM.length() + " bytes\n";
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
      output = F("All relays erased from EEPROM.");
    }
  }
  // else if (strncmp(com->args[0], "date", 4) == 0)
  //     output += clock.getDate());
  else if (com->args[0].equals("exit") || com->args[0].equals("quit"))
  {
    telnet.closeConnection();
  }
  else
  {
    output = F("Bad command: ");
    output += String(com->args[0]) + "\n";
  }

  return output;
  // output += "Free memory after: " + String(getFreeMemory()));
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
