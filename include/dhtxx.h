#ifndef DHTXX_H
#define DHTXX_H

#include <inttypes.h>

#define DHTXX_ERROR_OK 0
#define DHTXX_ERROR_COMM 1
#define DHTXX_ERROR_TIMEOUT 2
#define DHTXX_ERROR_OTHER 3
#define DHTXX_ERROR_CHECKSUM 4

#define DHTXX_DHT11 0
#define DHTXX_DHT22 1

extern uint8_t dhtxxread( unsigned char dev, volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, int *temp, int *humidity );

#endif
