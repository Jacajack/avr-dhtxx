#ifndef DHTXX_H
#define DHTXX_H

#include <inttypes.h>

uint8_t dhtxxreadb( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask );

#endif
