all:
	cd .. && make F_CPU=16000000U MCU=atmega328p
	avr-gcc -Wall --std=c99 -mmcu=atmega328p -DF_CPU=16000000U ../obj/dht11.o hhtest.c -Os -o hhtest.elf
 
