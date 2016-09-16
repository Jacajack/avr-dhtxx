#ifndef DHT22_H
#define DHT22_H

#include <inttypes.h>
#include "dhtxx.h"

uint8_t dht22read( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, int *temp, int *humidity );

#endif
