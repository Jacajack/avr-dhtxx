#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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

static uint8_t dhtxxreadb( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t *dest )
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
			if ( timeoutcnt++ > DHTXX_TIMEOUT ) return DHTXX_ERROR_TIMEOUT;
            _delay_us( 1 );
		}

		_delay_us( 30 );

		if ( *portin & mask ) data |= i;

		//Wait for the data part to finish
        timeoutcnt = 0;
		while ( *portin & mask )
		{
			if ( timeoutcnt++ > DHTXX_TIMEOUT ) return DHTXX_ERROR_TIMEOUT;
            _delay_us( 1 );
		}
	}

    *dest = data;

	return DHTXX_ERROR_OK;
}

uint8_t dhtxxread( unsigned char dev, volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, int *temp, int *humidity )
{
    uint8_t sreg = SREG; //Status register backup
	uint8_t data[5]; //Data received from sensor
    uint8_t cs = 0; //Checksum
    uint8_t ec = 0;
    uint8_t i;

    //Check if device type is correct
    if ( dev != DHTXX_DHT11 && dev != DHTXX_DHT22 ) return DHTXX_ERROR_OTHER;

	//Turn off pull-up, and send 20ms start signal
	*direction &= ~mask;
	*port &= ~mask;
	*direction |= mask;
	_delay_ms( 20 );

	//Turn pin into input and wait for acknowledgement
	*direction &= ~mask;
	_delay_us( 30 + 40 );

    //Communication check 1
	if ( *portin & mask )
	{
		SREG = sreg;
		return DHTXX_ERROR_COMM;
	}

	_delay_us( 80 );

    //Communication check 2
    if ( !( *portin & mask ) )
	{
		SREG = sreg;
		return DHTXX_ERROR_COMM;
	}

    _delay_us( 40 );

	//Read data from sensor
    for ( i = 0; i < 5; i++ )
    {
        ec = dhtxxreadb( port, direction, portin, mask, &data[i] );

        if ( ec )
        {
            SREG = sreg;
            return ec;
        }
    }

    //Checksum calculation
    for ( i = 0; i < 4; i++ )
        cs += data[i];
    if ( cs != data[4] )
    {
        SREG = sreg;
        return DHTXX_ERROR_CHECKSUM;
    }

    //Output values
    if ( dev == DHTXX_DHT22 ) //DHT22
    {
        *humidity = data[0] << 8 | data[1];
        *temp = data[2] << 8 | data[3];
    }
    else //DHT11
    {

    }

	return DHTXX_ERROR_OK;
}
