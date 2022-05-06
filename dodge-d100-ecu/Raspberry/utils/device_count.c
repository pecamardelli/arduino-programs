#include "../ecu.h"

int device_count() {
    int count = 0;
    DEVICE *aux = sys.first_device;

    while (aux) {
        count++;
        aux = aux->next;
    }
    
    return count;
}