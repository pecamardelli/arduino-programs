#include "header.h"

extern NodeList nodes;

System::System()
{
  configChanged = false;

  loadSystemData();

  // Array of special chars allowed on inputs
  specialChars[0] = 0x20;
  specialChars[1] = 0x2d;
  specialChars[2] = 0x2e;
  specialChars[3] = 0x2f;
  specialChars[4] = 0x3a;
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

String System::setHostname(char *newHostname)
{
  if (strlen(newHostname) == 0)
    return F("Error: hostname cannot be empty.");

  if (strlen(newHostname) > MAX_HOSTNAME_LEN)
    return F("Error: hostname exceedes maximum.");

  // Looks like we're good...
  strncpy(config.hostname, newHostname, MAX_HOSTNAME_LEN);

  return newHostname;
};

String System::setIpAddress(char *newIpAddress)
{
  if (!config.ethernetConfig.ipAddress.fromString(newIpAddress))
    return F("Error: invalid ip address.");

  Ethernet.setLocalIP(config.ethernetConfig.ipAddress);

  return newIpAddress;
};

String System::setSubnetMask(char *newSubnetMask)
{
  if (!config.ethernetConfig.subnetMask.fromString(newSubnetMask))
    return F("Error: invalid subnet mask.");

  Ethernet.setSubnetMask(config.ethernetConfig.subnetMask);

  return newSubnetMask;
};

String System::setDefaultGateway(char *newDefaultGateway)
{
  if (!config.ethernetConfig.gateway.fromString(newDefaultGateway))
    return F("Error: invalid default gateway.");

  Ethernet.setGatewayIP(config.ethernetConfig.gateway);

  return newDefaultGateway;
};

String System::setDnsServer(char *newDnsServer)
{
  if (!config.ethernetConfig.dnsServer.fromString(newDnsServer))
    return F("Error: invalid DNS server.");

  Ethernet.setDnsServerIP(config.ethernetConfig.dnsServer);

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

  if (strncmp(com->args[0], "help", 4) == 0)
  {
    output += F("time\t\t\t--> Returns the current time.\n");
    output += F("relay (pinNumber)\t--> Runs command on specified relay. Use 'relay help' for further options.\n");
    output += F("set (parameter)\t\t--> Sets a given parameter. Use 'set help' for further options.\n");
    output += F("show (parameter)\t--> Shows specified information. Use 'show help' for further options.\n");
    output += F("save\t\t\t--> Save changes.\n");
    output += F("exit\t\t\t--> Close connection (telnet only).\n");
    output += F("quit\t\t\t--> Close connection (telnet only).\n");
  }
  else if (strncmp(com->args[0], "set", 3) == 0)
  {
    if (strncmp(com->args[1], "help", 4) == 0 or !strlen(com->args[1]))
    {
      output += F("hostname\t\t--> Sets the system name.\n");
      output += F("ip\t\t--> Sets system's ip address.\n");
      output += F("mask\t\t--> Sets system's subnet mask.\n");
      output += F("gateway\t\t--> Sets system's default gateway.\n");
      output += F("dns\t\t--> Sets system's dns server.\n");
      output += F("mac\t\t--> Sets system's mac address.\n");
    }
    else if (strncmp(com->args[1], "hostname", 8) == 0)
      output = setHostname(com->args[2]); // By default, args[2] is an empty string. setHostname will deal with it.
    else if (strncmp(com->args[1], "ip", 2) == 0)
      output = setIpAddress(com->args[2]);
    else if (strncmp(com->args[1], "mask", 2) == 0)
      output = setSubnetMask(com->args[2]);
    else if (strncmp(com->args[1], "gateway", 2) == 0)
      output = setDefaultGateway(com->args[2]);
    else if (strncmp(com->args[1], "dns", 2) == 0)
      output = setDnsServer(com->args[2]);
    else if (strncmp(com->args[1], "mac", 2) == 0)
      output = F("Not implemented");

    else if (strncmp(com->args[0], "relay", 5) == 0)
    {
      output += F("Unknown subcommand \"");
      output += com->args[1];
      output += F("\"");
    }
  }
  else if (strncmp(com->args[0], "relay", 5) == 0)
  {
    if (strncmp(com->args[1], "help", 4) == 0 or !strlen(com->args[1]))
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
    else if (strncmp(com->args[1], "info", 4) == 0)
    {
      output = nodes.getRelayInfo();
      return output;
    }

    const uint8_t pin = atoi(com->args[1]);
    // output += "Pin: " + String(pin));

    if (strncmp(com->args[2], "create", 6) == 0)
    {
      output = nodes.createRelay(pin);
    }
    else if (strncmp(com->args[2], "on", 2) == 0)
    {
      Relay *relay = nodes.searchByPin(pin);
      if (relay == NULL)
      {
        output = F("No relay defined on pin ");
        output += pin;
        return output;
      }

      relay->setMode(LOW);

      output = F("Relay ");
      output += pin;
      output += F(" on");
    }
    else if (strncmp(com->args[2], "off", 3) == 0)
    {
      Relay *relay = nodes.searchByPin(pin);
      if (relay == NULL)
      {
        output = F("No relay defined on pin ");
        output += pin;
        return output;
      }

      relay->setMode(HIGH);

      output = F("Relay ");
      output += pin;
      output += F(" off");
    }
    else
    {
      output += F("Unknown subcommand: ");
      output += com->args[1];
      output += "\n";
    }
  }
  else if (strncmp(com->args[0], "freemem", 7) == 0)
  {
    output = F("Free memory: ");
    output += String(getFreeMemory());
    output += F(" bytes.");
  }
  else if (strncmp(com->args[0], "hostname", 8) == 0)
    output = config.hostname;
  else if (strncmp(com->args[0], "sysinfo", 7) == 0)
  {
    output = String(F("Hostname:\t")) + String(config.hostname) + "\n";
    output += String(F("IP Address:\t")) + ipToString(Ethernet.localIP()) + "\n";
    output += String(F("Subnet mask:\t")) + ipToString(Ethernet.subnetMask()) + "\n";
    output += String(F("Gateway:\t")) + ipToString(Ethernet.gatewayIP()) + "\n";
    output += String(F("DNS Server:\t")) + ipToString(Ethernet.dnsServerIP()) + "\n";
  }
  // else if (strncmp(com->args[0], "date", 4) == 0)
  //     output += clock.getDate());
  else if (strncmp(com->args[0], "exit", 4) == 0 || strncmp(com->args[0], "quit", 4) == 0)
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

  if (config.hostname[0] = 0xff)
    strcpy(config.hostname, "arduino");

  // Set a default mac address if it's not defined.
  if (config.ethernetConfig.macAddress[0] == 0xff)
  {
    config.ethernetConfig.macAddress[0] = 0xDE;
    config.ethernetConfig.macAddress[1] = 0xAD;
    config.ethernetConfig.macAddress[2] = 0xBE;
    config.ethernetConfig.macAddress[3] = 0xEF;
    config.ethernetConfig.macAddress[4] = 0xF0;
    config.ethernetConfig.macAddress[5] = 0x18;
  }

  // Set a default IP address if it's not defined.
  // If the first byte is 0, then no ip address has been stored.
  if (config.ethernetConfig.ipAddress[0] == 0xff)
  {
    config.ethernetConfig.ipAddress.fromString("192.168.40.8");
  }

  // Set a default subnet mask if it's not defined.
  // If the first byte is 0, then no mask has been stored.
  if (config.ethernetConfig.subnetMask[0] == 0xff)
  {
    config.ethernetConfig.subnetMask.fromString("255.255.255.0");
  }

  // Set a default gateway if it's not defined.
  // If the first byte is 0, then no gateway has been stored.
  if (config.ethernetConfig.gateway[0] == 0xff)
  {
    config.ethernetConfig.gateway.fromString("192.168.40.1");
  }

  // Set a default dns server if it's not defined.
  // If the first byte is 0, then no dns server has been stored.
  if (config.ethernetConfig.dnsServer[0] == 0xff)
  {
    config.ethernetConfig.dnsServer.fromString("179.42.171.21");
  }

  // Ethernet.begin(config.mac, config.ip, config.dns, config.gateway, config.subnet);

  // byte pos = 0;
  // relayQuantity = 0;
  // eeAddress = 0;

  // // All relays are loaded into a dynamic list. This way, more relays can be added later.

  // // From byte 0 to 100 is stored the "file system". Every two bytes is an integer
  // // that represents a memory address where the relay data is stored on the EEPROM.
  // for (byte i = 0; i < MAX_RELAY_NUMBER; i++)
  // {
  //   EEPROM.get(eeAddress, pos);
  // }

  // do
  // {
  //   node_t *aux = (node_t *)malloc(sizeof(node_t));

  //   if (!aux)
  //     break;
  //   else
  //   {
  //     EEPROM.get(eeAddress, aux->relay);

  //     if (aux->relay.type == 200)
  //     {
  //       if (!aux->relay.deleted)
  //       {
  //         if (first == NULL)
  //         {
  //           first = aux;
  //           first->next = NULL;
  //           last = first;
  //         }
  //         else
  //         {
  //           last->next = aux;
  //           last = aux;
  //           last->next = NULL;
  //         }

  //         pinMode(aux->relay.pin, OUTPUT);
  //         digitalWrite(aux->relay.pin, HIGH);
  //         aux->changeFlag = false;
  //         aux->overrided = false;

  //         relayQuantity++;
  //       }
  //       else
  //         free(aux);

  //       eeAddress += sizeof(relayData);
  //     }
  //     else
  //       break;
  //   }
  // } while (1);
};

String System::ipToString(IPAddress address)
{
  String stringifiedAddress;
  stringifiedAddress = String(address[0]) + ".";
  stringifiedAddress += String(address[1]) + ".";
  stringifiedAddress += String(address[2]) + ".";
  stringifiedAddress += String(address[3]);

  return stringifiedAddress;
}