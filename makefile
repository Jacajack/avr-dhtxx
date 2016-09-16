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

all: force obj/dht11.o obj/dht22.o

obj/tmp/dhtxx.o: src/dhtxx.o
	$(CC) $(CFLAGS) -c src/dhtxx.c -o obj/tmp/dhtxx.o

obj/dht11.o: obj/tmp/dhtxx.o src/dht11.c include/dht11.h
	$(CC) $(CFLAGS) -c src/dht11.c -o obj/tmp/dht11.o
	$(LD) $(LDFLAGS) -r obj/tmp/dht11.o obj/tmp/dhtxx.o -o obj/dht11.o
	avr-size -C --mcu=$(MCU) obj/dht11.o

obj/dht22.o: obj/tmp/dhtxx.o src/dht22.c include/dht22.h
	$(CC) $(CFLAGS) -c src/dht22.c -o obj/tmp/dht22.o
	$(LD) $(LDFLAGS) -r obj/tmp/dht22.o obj/tmp/dhtxx.o -o obj/dht22.o
	avr-size -C --mcu=$(MCU) obj/dht22.o

force:
	-mkdir obj
	-mkdir obj/tmp

clean:
	-rm -rf obj
