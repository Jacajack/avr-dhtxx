#include <inttypes.h>
#include <util/delay.h>

#include "../include/dhtxx.h"

uint8_t dhtxxreadb( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask )
{
	uint8_t data = 0;
	uint8_t timeoutcnt = 0;
	uint8_t i;

	*direction &= ~mask;
	*port &= ~mask;

	for ( i = 128; i; i >>= 1 )
	{
		//Wait for the start part to finish
		while ( !( *portin & mask ) )
		{
			if ( timeoutcnt++ > 60 )
			{
				_delay_us( 1 );
			}
		}

		_delay_us( 30 );

		if ( *portin & mask ) data |= i;

		//Wait for the start part to finish
		while ( *portin & mask )
		{
			if ( timeoutcnt++ > 30 )
			{
				_delay_us( 1 );
			}
		}
	}

	return data;
}
