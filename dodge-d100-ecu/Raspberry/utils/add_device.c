#include "../ecu.h"

void add_device(DEVICE *dev) {
    
    dev->next = NULL;
    sys.last_device = dev;
    
    // If device list is empty, make 'dev' the first and last entry.
    if (!sys.first_device) sys.first_device = dev;
}