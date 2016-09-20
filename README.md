# avr-dhtxx
[![The MIT License](https://img.shields.io/badge/license-MIT-orange.svg?style=flat-square)](http://opensource.org/licenses/MIT) [![Travis CI](https://img.shields.io/travis/Jacajack/avr-dhtxx.svg?style=flat-square)](https://travis-ci.org/Jacajack/avr-dhtxx)

`avr-dhtxx` is an lightweight (~550b) AVR library for reading data from DHT11 and DHT22 sensors.
It can be easily compiled for every micro-controller supported by `avr-gcc`.

Alright, here's some code:
```c
#include <avr/io.h>
#include <util/delay.h>
#include "../include/dhtxx.h"

int main( )
{
	unsigned char ec; //Exit code
	int temp, humid; //Temperature and humidity

	while( 1 )
	{
		//Request DHT sensor to give it time to prepare data
		dhtxxconvert( DHTXX_DHT22, &PORTC, &DDRC, &PINC, ( 1 << 0 ) );

		_delay_ms( 1000 );

		//Read data from sensor to variables `temp` and `humid` (`ec` is exit code)
		ec = dhtxxread( DHTXX_DHT22, &PORTC, &DDRC, &PINC, ( 1 << 0 ), &temp, &humid );
	}

	return 0;
}
```

As you can see, code is very simple. Library itself consists of two functions:
 - `dhtxxconvert( ... )` - sends request pulse to sensor in order to start data conversion.
 - `dhtxxread( ... )` - reads humidity and temperature data from sensor and triggers data conversion (data from previous conversion is returned).

Data received by `dhtxxread` is stored in two `int` variables specified by pointers passed as arguments to the function.
Temperature and humidity values returned are multiplied by 10. For instance, temperature value 230 actually means 23.0&deg;C.
Also, returned values come from last conversion, so if you want fresh data, `dhtxxconvert` should be called >1s before reading data with `dhtxxread`.
Anyway, always remember to take a look at datasheet.

Each function takes following arguments:
 - sensor type (`DHTXX_DHT11` or `DHTXX_DTH22`)
 - port output register address (e.g. `&PORTA` or `&PORTC`)
 - port direction register address (e.g. `&DDRA` or `&DDRD`)
 - port input register address (e.g. `&PINB` or `&PINC`)
 - a bit mask (e.g. `( 1 << 3 )` when sensor is connected to the pin 3)

And then, in case of `dhtxxread`:
 - pointer to an `int` variable for storing temperature data
 - pointer to an `int` variable for storing humidity data

It is worth mentioning, that both functions disable interrupts in timing-crucial code fragments, but before they exit `SREG` value is restored.

Every function also returns an error code (`uint8_t`):

|Value|Macro|Description|
|:---:|:---:|-----------|
|0|`DHTXX_ERROR_OK`|Everything is fine - no error|
|1|`DHTXX_ERROR_COMM`|Communication error occurred - sensor did not respond or transmission had been interrupted|
|2|`DHTXX_ERROR_CHECKSUM`|Received checksum is invalid|
|3|`DHTXX_ERROR_OTHER`|Some other error occurred - e.g. user tried to specify device other than `DHTXX_DHT11` or `DHTXX_DTH22`|

Library can be easily compiled using included `makefile`. `F_CPU` and `MCU` values should be specified when calling `make`:
`make F_CPU=16000000U MCU=atmega328p`
`DHTXX_TIMEOUT` value can be set too. It describes communication timeout in more or less microseconds. Default value is 60.

To understand how library works better, remember to take a look at `dhtxx.c` and `dhtxx.h`.
