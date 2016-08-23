GCC_DEVICE  = atmega328p
DUDE_DEVICE = m328p
CLOCK       = 8000000            # 8Mhz (this actually doesn't seem to make any difference)
PROGRAMMER  = -c usbtiny -P usb  # For using Sparkfun Pocket AVR Programmer
FUSES      = -U lfuse:w:0xe2:m -U hfuse:w:0xde:m -U efuse:w:0xff:m # CKDIV8 is off

AVRDUDE = avrdude $(PROGRAMMER) -p $(DUDE_DEVICE) -e
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(GCC_DEVICE)

all: main.hex

setup:
	mkdir -p -- build

clean:
	rm build/*

build/%.o : src/%.c
	$(COMPILE) -c $< -o $@

build/%.o : src/%.S
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

main.elf: build/main.o
	$(COMPILE) -o build/main.elf build/main.o

main.hex: setup main.elf
	avr-objcopy -j .text -j .data -O ihex build/main.elf build/main.hex
	avr-size --format=avr --mcu=$(GCC_DEVICE) build/main.elf

disasm: main.elf
	avr-objdump -d build/main.elf

upload: main.hex
	$(AVRDUDE) -U flash:w:build/main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

install: upload
