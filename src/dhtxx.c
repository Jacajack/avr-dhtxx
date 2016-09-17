#include <inttypes.h>
#include <util/delay.h>

#include "../include/dhtxx.h"

#ifndef DHTXX_TIMEOUT
#define DHTXX_TIMEOUT 60
#endif

#if DHTXX_TIMEOUT > 255
#error "DHTXX_TIMEOUT cannot take values greater than 255!"
#endif

/*
    _B_     _D_                _F_
_A_|   |_C_|     . . . . . _E_|

A - 20ms start signal
B - 20 - 40us response timeout
C - 80us presence acknowledgement
D - 80us of pause

E - 50us of sync impulse
F - 28us / 70us data impulse
*/

static uint8_t dhtxxreadb( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask )
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

uint8_t dhtxxread( unsigned char dev, volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, int *temp, int *humidity )
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

	/*somewhere here real communication begins...*/
	hint = dhtxxreadb( port, direction, portin, mask );
	hdec = dhtxxreadb( port, direction, portin, mask );
	tint = dhtxxreadb( port, direction, portin, mask );
	tdec = dhtxxreadb( port, direction, portin, mask );
	csum = dhtxxreadb( port, direction, portin, mask );

	*humidity = hint * 10 + hdec;
	*temp = tint * 10 + tdec;

	if ( csum != ( ( hint + hdec + tint + tdec ) & 0xFF ) ) return 1;
	return DHTXX_ERROR_OK;
}
