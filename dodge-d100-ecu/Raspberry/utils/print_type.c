#include "../ecu.h"

char *print_type(unsigned char type) {
    switch (type) {
        case DEV_RELAY:
            return "Relay";
            break;
        case DEV_FLOWMETER:
            return "Flowmeter";
            break;
        default:
            return "Unknown";
            break;
    }
}