#ifndef DHTXX_H
#define DHTXX_H

#include <inttypes.h>

#define DHTXX_ERROR_OK 0
#define DHTXX_ERROR_COMM 1
#define DHTXX_ERROR_TIMEOUT 2

extern uint8_t dhtxxread( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, int *temp, int *humidity );

#endif
