#include "../ecu.h"

unsigned char valid_device_type(unsigned char dev_type) {
    if (dev_type == DEV_RELAY)      return 0xFF;
    if (dev_type == DEV_FLOWMETER)  return 0xFF;
    return 0x0;
}