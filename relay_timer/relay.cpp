#include "header.h"

Relay::Relay(byte pin)
{
    if (searchRelay(pin))
        return;

    node_t *aux = (node_t *)malloc(sizeof(node_t));
    aux->next = NULL; // Set to NULL to inform that it's a new list entry.

    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);

    aux->relay.type = 200; // 200 is the standard number for a relay to be recongnized as such in the EEPROM
    aux->relay.pin = pin;
    aux->relay.enabled = false;
    sprintf(aux->relay.desc, "%s", "No description");
    aux->relay.startHour = 0;
    aux->relay.startMin = 0;
    aux->relay.endHour = 0;
    aux->relay.endMin = 0;
    aux->relay.deleted = false;
    aux->changeFlag = true;
    aux->overrided = false;

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
};

void Relay::createRelay(byte pin)
{
}

node_t *Relay::searchRelay(byte pin)
{
    if (!pin)
        return NULL;

    node_t *aux = first;
    while (aux != NULL)
    {
        if (aux->relay.pin == pin)
        {
            return aux;
        }
        aux = aux->next;
    }

    return NULL;
}

void Relay::checkRelays()
{
    DateTime now = clock.getTime();
    int startMins, endMins, currentMins;

    node_t *aux = first;
    while (aux != NULL)
    {
        if (aux->relay.enabled && !aux->relay.deleted && !aux->overrided)
        {
            startMins = aux->relay.startHour * 60 + aux->relay.startMin;
            endMins = aux->relay.endHour * 60 + aux->relay.endMin;
            currentMins = now.hour() * 60 + now.minute();

            if (startMins <= currentMins)
            {
                if (endMins > startMins)
                {
                    if (endMins < currentMins)
                    {
                        // digitalWrite(aux->relay.pin, HIGH);
                        switchRelay(aux->relay.pin, HIGH, false);
                    }
                    else
                    {
                        // digitalWrite(aux->relay.pin, LOW);
                        switchRelay(aux->relay.pin, LOW, false);
                    }
                }
                else
                {
                    // digitalWrite(aux->relay.pin, LOW);
                    switchRelay(aux->relay.pin, LOW, false);
                }
            }
            else
            {
                if (startMins < endMins)
                {
                    // digitalWrite(aux->relay.pin, HIGH);
                    switchRelay(aux->relay.pin, HIGH, false);
                }
                else
                {
                    if (endMins > currentMins)
                    {
                        // digitalWrite(aux->relay.pin, LOW);
                        switchRelay(aux->relay.pin, LOW, false);
                    }
                    else
                    {
                        // digitalWrite(aux->relay.pin, HIGH);
                        switchRelay(aux->relay.pin, HIGH, false);
                    }
                }
            }
        }
        else if (!aux->overrided)
        {
            // digitalWrite(aux->relay.pin, HIGH);
            switchRelay(aux->relay.pin, HIGH, false);
        }

        aux = aux->next;
    }
}

void Relay::deleteRelay(byte pin)
{
    node_t *target = searchRelay(pin);

    if (target)
    {
        target->relay.deleted = true;
        sys.printData(F("Relay successfully marked as deleted"), true);
    }
}

void Relay::setRelayStatus(byte pin, bool _status)
{
    node_t *target = searchRelay(pin);
    if (target)
    {
        target->relay.enabled = _status;
    }
}

void Relay::setRelayDesc(byte pin, char *_desc)
{
    node_t *target = searchRelay(pin);

    if (target)
    {
        if (strlen(_desc) <= RELAY_DESC_LEN)
            strcpy(target->relay.desc, _desc);
        else
            strncpy(target->relay.desc, _desc, RELAY_DESC_LEN);

        target->changeFlag = true;
        sys.printData(F("Description set to: "), false);
        sys.printData(target->relay.desc, true);
    }
}

void Relay::setRelayParams(byte pin, char *_param, byte type)
{
    node_t *target = searchRelay(pin);
    int param;
    sscanf(_param, "%d", &param);

    if (target)
    {
        switch (type)
        {
        case TYPE_START_HOUR:
            if (param >= 0 && param <= 23)
            {
                target->relay.startHour = param;
                target->changeFlag = true;
                sys.printData(F("Starting hour set to: "), false);
                sys.printData(String(param), true);
            }
            else
            {
                sys.printData(F("ERROR! Invalid starting hour \""), false);
                sys.printData(String(param) + "\"", true);
                return;
            }
            break;
        case TYPE_END_HOUR:
            if (param >= 0 && param <= 23)
            {
                target->relay.endHour = param;
                target->changeFlag = true;
                sys.printData("Pin " + String(pin), false);
                sys.printData(F(": ending hour set to: "), false);
                sys.printData(String(param), true);
            }
            else
            {
                sys.printData(F("ERROR! Invalid ending hour \""), false);
                sys.printData(String(param) + "\"", true);
                return;
            }
            break;
        case TYPE_START_MIN:
            if (param >= 0 && param <= 59)
            {
                target->relay.startMin = param;
                target->changeFlag = true;
                sys.printData("Pin " + String(pin), false);
                sys.printData(F(": starting minute set to: "), false);
                sys.printData(String(param), true);
            }
            else
            {
                sys.printData(F("ERROR! Invalid starting minute \""), false);
                sys.printData(String(param) + "\"", true);
                return;
            }
            break;
        case TYPE_END_MIN:
            if (param >= 0 && param <= 59)
            {
                target->relay.endMin = param;
                target->changeFlag = true;
                sys.printData("Pin " + String(pin), false);
                sys.printData(F(": ending minute set to: "), false);
                sys.printData(String(param), true);
            }
            else
            {
                sys.printData(F("ERROR! Invalid ending minute \""), false);
                sys.printData(String(param) + "\"", true);
                return;
            }
            break;
        default:
            sys.printData(F("ERROR! Invalid param type \""), true);
        }
    }
}

void Relay::resumeRelay(byte pin)
{
    node_t *target = searchRelay(pin);

    if (target)
    {
        target->overrided = false;
        sys.printData("Relay " + String(pin), false);
        sys.printData(F(" resumed."), true);
    }
}

void Relay::changeRelayPin(byte currentPin, char *_newPin)
{
    // Check if the new pin is valid
    const byte newPin = (byte)_newPin;
    // Now check if someone else has it.
    node_t *isUsed = searchRelay(newPin);

    if (isUsed)
        return sys.printData(F("Error: new pin already in use."), true);

    // Looks like we're good.
    node_t *target = searchRelay(currentPin);

    if (target)
    {
        target->relay.pin = newPin;
        target->changeFlag = true;
        sys.printData(F("New pin successfully changed to: "), false);
        sys.printData(String(newPin), true);
    }
}

void Relay::switchRelay(byte pin, uint8_t state, bool overrided)
{
    node_t *target = searchRelay(pin);

    if (target)
    {
        if (digitalRead(target->relay.pin) == HIGH && state == LOW)
            target->relay.startTime = millis();
        if (digitalRead(target->relay.pin) == LOW && state == HIGH)
            target->relay.startTime = 0;

        digitalWrite(target->relay.pin, state);
        target->overrided = overrided;
    }
}

void Relay::getRelayInfo()
{
    sys.printData(F("PIN\tENABLED\tDESCRIPTION\t\SWITCH ON\tSWITCH OFF\tSTATUS\tUPTIME"), true);
    node_t *aux = first;

    while (aux != NULL)
    {
        if (!aux->relay.deleted)
        {
            sys.printData(String(aux->relay.pin) + "\t", false);

            const char *_enabled = (aux->relay.enabled) ? "true" : "false";
            sys.printData(_enabled, false);
            sys.printData("\t", false);
            sys.printData(aux->relay.desc, false);

            size_t descLen = strlen(aux->relay.desc);

            if (descLen < 8)
                sys.printData("\t\t\t", false);
            else if (descLen < 16)
                sys.printData("\t\t", false);
            else
                sys.printData("\t", false);

            String starth = (aux->relay.startHour < 10) ? "0" + String(aux->relay.startHour) : String(aux->relay.startHour);
            String startm = (aux->relay.startMin < 10) ? "0" + String(aux->relay.startMin) : String(aux->relay.startMin);
            sys.printData(starth + ":" + startm + "\t", false);

            String endh = (aux->relay.endHour < 10) ? "0" + String(aux->relay.endHour) : String(aux->relay.endHour);
            String endm = (aux->relay.endMin < 10) ? "0" + String(aux->relay.endMin) : String(aux->relay.endMin);
            sys.printData(endh + ":" + endm + "\t", false);

            const char *_mode = digitalRead(aux->relay.pin) ? "off" : "on";
            sys.printData(_mode, false);
            sys.printData("\t", false);

            if (aux->relay.startTime)
                sys.printData(parseUpTime(aux->relay.startTime), false);
            else
                sys.printData(F("--d --:--:--"), false);

            sys.printData("\t", false);

            if (aux->changeFlag)
                sys.printData("*", true);
            else
                sys.printData("", true);
        }
        aux = aux->next;
    }
}
