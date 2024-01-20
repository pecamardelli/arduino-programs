#include "System.h"

System::System(/* args */)
{
}

System::~System()
{
}

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

void System::loadSystemData()
{
    // LOAD SYSTEM DATA
    EEPROM.get(0, systemData);

    if (systemData.hostname[0] == 0xff)
        strcpy(systemData.hostname, "arduino");
};

void System::saveSystemData()
{
    EEPROM.put(0, systemData);
}

sys_data System::getSystemData()
{
    return systemData;
}

void System::updateSystemData(sys_data updatedData)
{
    systemData = updatedData;
}
