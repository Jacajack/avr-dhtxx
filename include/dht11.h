#ifndef DHT11_H
#define DHT11_H

#include <inttypes.h>
#include "dhtxx.h"

extern uint8_t dht11read( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, unsigned char *temp, unsigned char *humidity );

#endif
