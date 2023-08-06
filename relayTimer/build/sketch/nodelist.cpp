#line 1 "/home/pablin/arduino_programs/relayTimer/nodelist.cpp"
#include "header.h"

NodeList::NodeList(/* args */)
{
    lastCheckTimeStamp = 0;
}

NodeList::~NodeList()
{
}

uint8_t NodeList::searchByPin(uint8_t pin)
{
    for (uint8_t i = 0; i < MAX_RELAY_NUMBER; i++)
    {
        if (relayArray[i].getPin() == pin)
        {
            return i;
        }
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

    return NULL;
}

bool NodeList::isPinAvailable(uint8_t pin)
{
    if (pin <= 1 || pin >= DIGITAL_PINS)
        return false;

    if (searchByPin(pin) != NULL)
        return false;

    return true;
}

void NodeList::checkRelays()
{
    if (millis() - lastCheckTimeStamp < sys.config.relayCheckInterval)
        return;

    //   DateTime now = clock.RTC.now();
    uint16_t startMins, endMins, currentMins;

    for (uint8_t i = 0; i < MAX_RELAY_NUMBER; i++)
    {
        if (relayArray[i].getStatus() == enabled && !relayArray[i].overrided)
        {
            startMins = relayArray[i].getStartHour() * 60 + relayArray[i].getStartMinute();
            endMins = relayArray[i].getEndHour() * 60 + relayArray[i].getEndMinute();
            currentMins = 720; // now.hour() * 60 + now.minute();

            if (startMins <= currentMins)
            {
                if (endMins > startMins)
                {
                    if (endMins < currentMins)
                    {
                        // switchRelay(aux->relay.pin, HIGH, false);
                        relayArray[i].switchOff();
                    }
                    else
                    {
                        // switchRelay(aux->relay.pin, LOW, false);
                        relayArray[i].switchOn();
                    }
                }
                else
                {
                    // switchRelay(aux->relay.pin, LOW, false);
                    relayArray[i].switchOn();
                }
            }
            else
            {
                if (startMins < endMins)
                {
                    // switchRelay(aux->relay.pin, HIGH, false);
                    relayArray[i].switchOff();
                }
                else
                {
                    if (endMins > currentMins)
                    {
                        // switchRelay(aux->relay.pin, LOW, false);
                        relayArray[i].switchOn();
                    }
                    else
                    {
                        // switchRelay(aux->relay.pin, HIGH, false);
                        relayArray[i].switchOff();
                    }
                }
            }
        }
        else if (!relayArray[i].overrided)
        {
            // switchRelay(aux->relay.pin, HIGH, false);
            relayArray[i].switchOff();
        }
    }

    lastCheckTimeStamp = millis();
}

String NodeList::getRelayInfo()
{
    String output = F("PIN\tENABLED\tDESCRIPTION\t\tSTART\tEND\tSTATUS\tUPTIME\n");

    for (uint8_t i = 0; i < MAX_RELAY_NUMBER; i++)
    {
        output += String(relayArray[i].getPin()) + "\t";

        const char *_enabled = (relayArray[i].getStatus()) ? "true" : "false";
        output += String(_enabled) + "\t" + String(relayArray[i].getDesc());

        const size_t descLen = relayArray[i].getDesc().length();

        if (descLen < 8)
            output += "\t\t\t";
        else if (descLen < 16)
            output += "\t\t";
        else
            output += "\t";

        output += relayArray[i].getStartTime() + "\t";
        output += relayArray[i].getEndTime() + "\t";

        String _mode = digitalRead(relayArray[i].getPin()) ? "off" : "on";
        output += _mode + "\t";
        output += relayArray[i].getUptime() + "\t";
        output += "\n";
    }

    return output;
}

void NodeList::saveRelay(Relay *relay)
{
    relayData_t relayData = relay->getData();
    uint16_t eeAddress = relay->getEepromAddress();
    EEPROM.put(eeAddress, relayData);
}

void NodeList::loadRelays()
{
    Serial.println(F("Loading relays..."));
    uint16_t eeAddress = 0;

    relayData_t relayData;
    for (uint8_t i = 0; i < MAX_RELAY_NUMBER; i++)
    {
        EEPROM.get(eeAddress, relayData);
        relayArray[i].setParams(relayData);
        eeAddress += sizeof(relayData_t);
    }
}

void NodeList::eraseRelaysFromEEPROM()
{
    for (uint16_t i = 0; i < sizeof(relayData_t) * MAX_RELAY_NUMBER; i++)
    {
        EEPROM.write(i, 0xff);
    }
}