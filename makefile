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

all: force obj/dht11.o

obj/dht11.o: src/dht11.c include/dht11.h
	$(CC) $(CFLAGS) -c src/dht11.c -o obj/dht11.o
	avr-size -C --mcu=$(MCU) obj/dht11.o

obj/dht22.o: src/dht22.c include/dht22.h
	$(CC) $(CFLAGS) -c src/dht22.c -o obj/dht22.o
	avr-size -C --mcu=$(MCU) obj/dht22.o

force:
	-mkdir obj

clean:
	rm -rf obj
