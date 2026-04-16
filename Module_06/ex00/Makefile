CC = avr-gcc

NAME = main

SRC = main.c uart/uart.c

F_CPU = 16000000

RM = rm -rf

CFLAGS = -Os -Wall -Wextra -Werror -Iuart -mmcu=atmega328p -DF_CPU=$(F_CPU)

ELF = $(NAME).elf
HEX = $(NAME).hex
BIN = $(NAME).bin

OBJS = $(SRC:%.c=%.o)

all: hex flash

hex: $(ELF)
	avr-objcopy -O ihex $^ $(HEX)

$(ELF): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

flash:
	avrdude -p m328p -b 115200 -c arduino  -P /dev/ttyUSB0 -U flash:w:$(HEX)

clean:
	$(RM) $(OBJS) $(ELF) $(HEX)

re: clean all
