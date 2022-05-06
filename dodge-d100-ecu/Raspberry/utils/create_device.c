#include "../ecu.h"

DEVICE *create_device(unsigned char type, unsigned char enabled, u_int8_t pin, char *desc) {
    if (!valid_device_type(type)) {
        logger("Failed to create device: Invalid type.", LOG_ERROR);
        return NULL;
    }

    if (!pin_available(pin)) {
        logger("Failed to create device: Pin not available.", LOG_ERROR);
        return NULL;
    }

    if (strlen(desc) > MAX_LINE_LEN) {
        logger("Failed to create device: Description too long.", LOG_ERROR);
        return NULL;
    }

    DEVICE *dev = (DEVICE*) malloc (sizeof(DEVICE));

    dev->type       = type;
    dev->enabled    = enabled;
    dev->pin        = pin;
    strcpy(dev->desc, desc);

    return dev;
}