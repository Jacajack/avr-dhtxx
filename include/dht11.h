#ifndef DHT11_H
#define DHT11_H

#include <inttypes.h>

#define DHTXX_ERROR_OK 0
#define DHTXX_ERROR_COMM 1

extern uint8_t dht11read( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, unsigned char *temp, unsigned char *humidity );

#endif
