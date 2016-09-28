#   makefile -  a part of avr-dhtxx library
#
#   Copyright (C) 2016 Jacek Wieczorek
#
#   This software may be modified and distributed under the terms
#   of the MIT license.  See the LICENSE file for details.


F_CPU =
MCU =
DHTXX_TIMEOUT =

CC = avr-gcc
CFLAGS = -Wall -DDHTXX_TIMEOUT=$(DHTXX_TIMEOUT) -DF_CPU=$(F_CPU) -mmcu=$(MCU) -Os

LD = avr-ld
LDFLAGS =

ifneq ($(MAKECMDGOALS),clean)
ifndef F_CPU
$(error F_CPU is not set!)
endif
ifndef MCU
$(error MCU is not set!)
endif
ifndef DHTXX_TIMEOUT
$(warning DHTXX_TIMEOUT not set! Default value is 60...)
DHTXX_TIMEOUT = 60
endif
endif

all: clean force obj/dhtxx.o lib/libdhtxx.a end

lib/libdhtxx.a: obj/dhtxx.o
	avr-ar -cvq lib/libdhtxx.a obj/dhtxx.o
	avr-ar -t  lib/libdhtxx.a

obj/dhtxx.o: src/dhtxx.c
	$(CC) $(CFLAGS) -c src/dhtxx.c -o obj/dhtxx.o

force:
	-mkdir obj
	-mkdir obj/tmp
	-mkdir lib

clean:
	-rm -rf obj
	-rm -rf lib

end:
	avr-size -C --mcu=$(MCU) obj/dhtxx.o
