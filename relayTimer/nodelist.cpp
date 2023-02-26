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

String NodeList::createRelay(uint8_t pin)
{
    if (sys.getFreeMemory() <= sizeof(node_t))
        return "Not enough memory.";

    if (!isPinAvailable(pin))
        return F("Error: pin not available");

    node_t *aux = (node_t *)malloc(sizeof(node_t));

    if (!aux)
        return "ERROR! Could not allocate memory.";

    aux->next = NULL; // Set to NULL to inform that it's a new list entry.

    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);

    aux->relay = new Relay(pin);

    if (aux->next == NULL)
    {
        if (first == NULL)
        {
            first = aux;
            last = aux;
        }
        else
        {
            last->next = aux;
            last = aux;
            // last->next  = NULL;  In this case, next has been set to NULL when memory was allocated.
        }
    }

    return F("Relay created.");
}

void NodeList::checkRelays()
{
    if (millis() - lastCheckTimeStamp < sys.config.relayCheckInterval)
        return;

    //   DateTime now = clock.RTC.now();
    int startMins, endMins, currentMins;

    node_t *aux = first;
    while (aux != NULL)
    {
        if (aux->relay->getStatus() == enabled && !aux->overrided)
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
        else if (!aux->overrided)
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

        if (aux->changeFlag)
            output += "*";

        output += "\n";

        aux = aux->next;
    }

    return output;
}
