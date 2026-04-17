CC = avr-gcc

NAME = main

SRC = main.c uart/uart.c i2c/i2c.c 

F_CPU = 16000000

BAUD = 115200

PORT ?= /dev/ttyUSB0

RM = rm -rf

CPPFLAGS = -Iuart -Ii2c -DF_CPU=$(F_CPU) -DFOSC=$(F_CPU) -DBAUD=$(BAUD)
CFLAGS = -Os -Wall -Wextra -Werror -mmcu=atmega328p 

ELF = $(NAME).elf
HEX = $(NAME).hex
BIN = $(NAME).bin

OBJS = $(SRC:%.c=%.o)

.PHONY: all clean re flash hex bin

all: hex

hex: $(ELF)
	avr-objcopy -O ihex $< $(HEX)

bin: $(ELF)
	avr-objcopy -O binary $< $(BIN)

$(ELF): $(OBJS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

flash: hex
	avrdude -p m328p -b $(BAUD) -c arduino -P $(PORT) -U flash:w:$(HEX)

clean:
	$(RM) $(OBJS) $(ELF) $(HEX) $(BIN)

re: clean all
