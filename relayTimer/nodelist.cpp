#include "header.h"

NodeList::NodeList(/* args */)
{
    first = NULL;
    last = NULL;
    lastCheckTimeStamp = 0;
}

NodeList::~NodeList()
{
}

Relay *NodeList::searchByPin(uint8_t pin)
{

    node_t *aux = first;
    while (aux != NULL)
    {
        if (aux->relay->getPin() == pin)
        {
            return aux->relay;
        }
        aux = aux->next;
    }

    return NULL;
}

uint8_t NodeList::getAvailablePin()
{
    // Leaving pins 1, 2 and 13 unused
    for (uint8_t k = 2; k < DIGITAL_PINS - 1; k++)
    {
        if (searchByPin(k) == NULL)
            return k;
    }

    return 0;
}

bool NodeList::isPinAvailable(uint8_t pin)
{
    if (pin <= 1 || pin >= DIGITAL_PINS)
        return false;

    if (searchByPin(pin) != NULL)
        return false;

    return true;
}

Relay *NodeList::createRelay(uint8_t pin)
{
    if (!isPinAvailable(pin))
        // return F("Error: pin not available");
        return NULL;

    uint16_t newRelayEepromAddress = EEPROM_RESERVED_BYTES + getNodeNumber() * (sizeof(relayData_t) + 1);

    Relay *newRelay = new Relay(pin, newRelayEepromAddress);

    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);

    saveRelay(newRelay);

    return newRelay;
}

String NodeList::addNode(Relay *relay)
{
    Serial.println("Adding a new node...");
    if (sys.getFreeMemory() <= sizeof(node_t))
    {
        delete relay;
        return F("Not enough memory.");
    }

    node_t *aux = (node_t *)malloc(sizeof(node_t));

    if (!aux)
    {
        delete relay;
        return "ERROR! Could not allocate memory.";
    }

    aux->next = NULL;
    aux->relay = relay;

    // if (aux->next == NULL)
    // {
    if (first == NULL)
    {
        first = aux;
        last = aux;
    }
    else
    {
        last->next = aux;
        last = aux;
    }
    // }

    return F("Node added.");
}

void NodeList::checkRelays()
{
    if (millis() - lastCheckTimeStamp < sys.config.relayCheckInterval)
        return;

    //   DateTime now = clock.RTC.now();
    uint16_t startMins, endMins, currentMins;

    node_t *aux = first;
    while (aux != NULL)
    {
        if (aux->relay->getStatus() == enabled && !aux->relay->overrided)
        {
            startMins = aux->relay->getStartHour() * 60 + aux->relay->getStartMinute();
            endMins = aux->relay->getEndHour() * 60 + aux->relay->getEndMinute();
            currentMins = 720; // now.hour() * 60 + now.minute();

            if (startMins <= currentMins)
            {
                if (endMins > startMins)
                {
                    if (endMins < currentMins)
                    {
                        // switchRelay(aux->relay.pin, HIGH, false);
                        aux->relay->switchOff();
                    }
                    else
                    {
                        // switchRelay(aux->relay.pin, LOW, false);
                        aux->relay->switchOn();
                    }
                }
                else
                {
                    // switchRelay(aux->relay.pin, LOW, false);
                    aux->relay->switchOn();
                }
            }
            else
            {
                if (startMins < endMins)
                {
                    // switchRelay(aux->relay.pin, HIGH, false);
                    aux->relay->switchOff();
                }
                else
                {
                    if (endMins > currentMins)
                    {
                        // switchRelay(aux->relay.pin, LOW, false);
                        aux->relay->switchOn();
                    }
                    else
                    {
                        // switchRelay(aux->relay.pin, HIGH, false);
                        aux->relay->switchOff();
                    }
                }
            }
        }
        else if (!aux->relay->overrided)
        {
            // switchRelay(aux->relay.pin, HIGH, false);
            aux->relay->switchOff();
        }

        aux = aux->next;
    }

    lastCheckTimeStamp = millis();
}

String NodeList::getRelayInfo()
{
    String output = F("PIN\tENABLED\tDESCRIPTION\t\tSTART\tEND\tSTATUS\tUPTIME\n");
    node_t *aux = first;

    while (aux != NULL)
    {
        output += String(aux->relay->getPin()) + "\t";

        const char *_enabled = (aux->relay->getStatus()) ? "true" : "false";
        output += String(_enabled) + "\t" + String(aux->relay->getDesc());

        const size_t descLen = aux->relay->getDesc().length();

        if (descLen < 8)
            output += "\t\t\t";
        else if (descLen < 16)
            output += "\t\t";
        else
            output += "\t";

        output += aux->relay->getStartTime() + "\t";
        output += aux->relay->getEndTime() + "\t";

        String _mode = digitalRead(aux->relay->getPin()) ? "off" : "on";
        output += _mode + "\t";
        output += aux->relay->getUptime() + "\t";
        output += "\n";

        aux = aux->next;
    }

    return output;
}

uint8_t NodeList::getNodeNumber()
{
    uint8_t relayCount = 0;
    node_t *aux = first;

    while (aux != NULL)
    {
        relayCount++;
        aux = aux->next;
    }

    return relayCount;
}

void NodeList::saveRelay(Relay *relay)
{
    relayData_t relayData = relay->getData();
    uint16_t eeAddress = relay->getEepromAddress();

    Serial.print("Relay pin: ");
    Serial.print(relayData.pin);
    Serial.print(" - Address: ");
    Serial.println(eeAddress);

    EEPROM.put(eeAddress, relayData);
}

void NodeList::loadRelays()
{
    Serial.println(F("Loading relays..."));
    // Starting memory position.
    uint16_t eeAddress = EEPROM_RESERVED_BYTES;
    uint8_t eeAddressStep = sizeof(relayData_t) + 1;
    uint8_t totalRelaysLoaded = 0;

    relayData_t relayData;
    EEPROM.get(eeAddress, relayData);

    Serial.print("Expected identifier: ");
    Serial.print(RELAY_IDENTIFIER);
    Serial.print(" - Received: ");
    Serial.print(relayData.identifier);
    Serial.print(" - Status: ");
    Serial.println(relayData.status);

    while (relayData.identifier == RELAY_IDENTIFIER && relayData.status != deleted)
    {
        Relay *relay = new Relay(relayData, eeAddress);

        addNode(relay);

        totalRelaysLoaded++;

        eeAddress += eeAddressStep;
        EEPROM.get(eeAddress, relayData);

        Serial.print("Expected identifier: ");
        Serial.print(RELAY_IDENTIFIER);
        Serial.print(" - Received: ");
        Serial.print(relayData.identifier);
        Serial.print(" - Status: ");
        Serial.println(relayData.status);
    }

    Serial.print(F("Total relays loaded: "));
    Serial.println(totalRelaysLoaded);
}

void NodeList::eraseRelaysFromEEPROM()
{
    uint16_t startAddress = EEPROM_RESERVED_BYTES;
    uint16_t endAddress = EEPROM.length() - sizeof(system_t) - 1;

    for (uint16_t i = startAddress; i < endAddress; i++)
    {
        EEPROM.write(i, 0xff);
    }
}