#   makefile -  a part of avr-dhtxx library
#
#   Copyright (C) 2016 Jacek Wieczorek
#
#   This software may be modified and distributed under the terms
#   of the MIT license.  See the LICENSE file for details.


F_CPU =
MCU =

CC = avr-gcc
CFLAGS = -Wall -DF_CPU=$(F_CPU) -mmcu=$(MCU) -Os

LD = avr-ld
LDFLAGS =

ifndef F_CPU
$(error F_CPU is not set!)
endif

ifndef MCU
$(error MCU is not set!)
endif

all: force obj/dhtxx.o

obj/dhtxx.o: src/dhtxx.o
	$(CC) $(CFLAGS) -c src/dhtxx.c -o obj/dhtxx.o
	avr-size -C --mcu=$(MCU) obj/dhtxx.o

force:
	-mkdir obj
	-mkdir obj/tmp

clean:
	-rm -rf obj
