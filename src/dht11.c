#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

#include "../include/dht11.h"

unsigned char dht11read( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t *hum, uint8_t *temp )
{
	
	return DHTXX_ERROR_OK;
}
