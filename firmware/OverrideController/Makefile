F_CPU=1000000
CC=avr-gcc
MCU=attiny85
PROG=usbasp

.PHONY: all
all: main.hex

flash: main.hex
	avrdude -p $(MCU) -c $(PROG) -U flash:w:main.hex:i

main.hex: main.c
	$(CC) -o main.o -Os -mmcu=$(MCU) -c main.c -DF_CPU=$(F_CPU)
	$(CC) -mmcu=$(MCU) $(OBJS) main.o -o main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

.PHONY: clean
clean:
	rm -f main.o main.elf main.hex

.PHONY: set_fuses
set_fuses:
	avrdude -c $(PROG) -p $(MCU) -U lfuse:w:0xE2:m -U hfuse:w:0xDF:m -U efuse:w:0xFF:m