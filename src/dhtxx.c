/* dhtxx.c - a part of avr-dhtxx library
 *
 * Copyright (C) 2016 Jacek Wieczorek
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.	See the LICENSE file for details.
 */

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
- 20ms start signal
- 20 - 40us response timeout
- 80us presence acknowledgement
- 80us of pause

- 50us of sync impulse
- 28us / 70us data impulse
*/

static uint8_t dhtxxreadb( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t *dest )
{
	uint8_t sreg = SREG; //Backup status register
	uint8_t data = 0;
	uint8_t timeoutcnt = 0;
	uint8_t i = 0;

	//Disable interrupts
	cli( );

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
			if ( timeoutcnt++ > DHTXX_TIMEOUT )
			{
				SREG = sreg;
				return DHTXX_ERROR_COMM;
			}
			_delay_us( 1 );
		}

		_delay_us( 30 );

		if ( *portin & mask ) data |= i;

		//Wait for the data part to finish
		timeoutcnt = 0;
		while ( *portin & mask )
		{
			if ( timeoutcnt++ > DHTXX_TIMEOUT )
			{
				SREG = sreg;
				return DHTXX_ERROR_COMM;
			}
			_delay_us( 1 );
		}
	}

	*dest = data;

	SREG = sreg;
	return DHTXX_ERROR_OK;
}

uint8_t dhtxxread( unsigned char dev, volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, int *temperature, int *humidity )
{
	uint8_t sreg = SREG; //Status register backup
	uint8_t data[5]; //Data received from sensor
	uint8_t cs = 0; //Checksum
	uint8_t ec = 0;
	uint8_t i;

	//Check if device type is correct
	if ( dev != DHTXX_DHT11 && dev != DHTXX_DHT22 ) return DHTXX_ERROR_OTHER;

	//Send start signal
	*direction &= ~mask;
	*port &= ~mask;
	*direction |= mask;

	//Adjust start signal time for DHT11 and DHT22
	if ( dev == DHTXX_DHT22 )
		_delay_us( 500 );
	else
		_delay_ms( 18 );

	//Turn pin into input, disable interrupts and wait for acknowledgement
	cli( );
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

	//Restore interrupts status
	SREG = sreg;
	_delay_us( 40 );

	//Read data from sensor
	for ( i = 0; i < 5; i++ )
	{
		ec = dhtxxreadb( port, direction, portin, mask, &data[i] );
		if ( ec ) return ec;
	}

	//Checksum calculation
	for ( i = 0; i < 4; i++ )
		cs += data[i];
	if ( cs != data[4] )
		return DHTXX_ERROR_CHECKSUM;

	//Output values
	if ( dev == DHTXX_DHT22 ) //DHT22
	{
		//Will return humidity*10 and temperature*10
		*humidity = data[0] << 8 | data[1];
		*temperature = data[2] << 8 | data[3];
	}
	else //DHT11
	{
		*humidity = data[0] * 10;
		*temperature = data[2] * 10;
	}

	return DHTXX_ERROR_OK;
}

uint8_t dhtxxconvert( unsigned char dev, volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask )
{
	//Requests DHTxx device, but does not read the data (simply triggers conversion)

	uint8_t sreg = SREG; //Status register backup

	//Check if device type is correct
	if ( dev != DHTXX_DHT11 && dev != DHTXX_DHT22 ) return DHTXX_ERROR_OTHER;

	//Send start signal
	*direction &= ~mask;
	*port &= ~mask;
	*direction |= mask;

	//Adjust start signal time for DHT11 and DHT22
	if ( dev == DHTXX_DHT22 )
		_delay_us( 500 );
	else
		_delay_ms( 18 );

	//Turn pin into input, disable interrupts and wait for acknowledgement
	cli( );
	*direction &= ~mask;
	_delay_us( 30 + 40 );

	//Communication check 1
	if ( *portin & mask )
	{
		SREG = sreg;
		return DHTXX_ERROR_COMM;
	}

	SREG = sreg;
	return DHTXX_ERROR_OK;
}
