CC = avr-gcc

NAME = main

SRC = $(addsuffix .c, $(NAME))

F_CPU = 8000000

RM = rm -rf

CFLAGS = -Os -Wall -Wextra -Werror -DF_CPU=$(F_CPU) -mmcu=atmega328p

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
