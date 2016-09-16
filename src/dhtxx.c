#include <inttypes.h>
#include <util/delay.h>

#include "../include/dhtxx.h"

#ifndef DHTXX_TIMEOUT
#define DHTXX_TIMEOUT 60
#endif

#if DHTXX_TIMEOUT > 255
#error "DHTXX_TIMEOUT cannot take values greater than 255!"
#endif

uint8_t dhtxxreadb( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask )
{
	uint8_t data = 0;
	uint8_t timeoutcnt = 0;
	uint8_t i = 0;

    //Turn pin into input
	*direction &= ~mask;
	*port &= ~mask;

    //Get 8 bits
	for ( i = 128; i; i >>= 1 )
	{
		//Wait for the sync part to finish
        timeoutcnt = 0;
		while ( !( *portin & mask ) )
		{
			if ( timeoutcnt++ > DHTXX_TIMEOUT ) return 0;
            _delay_us( 1 );
		}

		_delay_us( 30 );

		if ( *portin & mask ) data |= i;

		//Wait for the data part to finish
        timeoutcnt = 0;
		while ( *portin & mask )
		{
			if ( timeoutcnt++ > DHTXX_TIMEOUT ) return 0;
            _delay_us( 1 );
		}
	}

	return data;
}
