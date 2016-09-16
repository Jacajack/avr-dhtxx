#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

#include "../include/dht11.h"

uint8_t dhtxxreadb( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask )
{
	uint8_t data = 0;
	uint8_t i;

	*direction &= ~mask;
	*port &= ~mask;

	for ( i = 1; i; i <<= 1 )
	{
		_delay_us( 25 + 14 ); //Wait to the half of short data
		_delay_us( 25 + 14 ); //Wait to the half of next sync or still lasting long data

		if ( *portin & mask )
		{
			data |= i;
			_delay_us( 42 ); //Wait for next half of sync
		}
	}

	return data;
}

uint8_t dht11read( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, unsigned char *temp, unsigned char *humidity )
{
	uint8_t tdec, tint, hdec, hint, csum;

	//Turn off pull-up, and send 20ms start signal
	*direction &= ~mask;
	*port &= ~mask;
	*direction |= mask;
	_delay_ms( 20 );

	//Turn pin into input and wait for acknowledgement
	*direction &= ~mask;
	_delay_us( 30 + 40 );

	if ( *portin & mask )
	{
		//TODO: exit properly here
		return DHTXX_ERROR_COMM;
	}

	_delay_us( 40 + 80 );
	_delay_us( 25 ); //Wait till the half of sync

	/*somewhere here real communication begins...*/
	hint = dhtxxreadb( port, direction, portin, mask );
	hdec = dhtxxreadb( port, direction, portin, mask );
	tint = dhtxxreadb( port, direction, portin, mask );
	tdec = dhtxxreadb( port, direction, portin, mask );
	csum = dhtxxreadb( port, direction, portin, mask );

	*humidity = hdec;
	*temp = tdec;

	if ( csum != ( ( hint + hdec + tint + tdec ) & 0xFF ) ) return 1;
	return DHTXX_ERROR_OK;
}
