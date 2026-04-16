#ifndef UART_H
#define UART_H

#include <avr/io.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 115200   // Baud Rate

void uart_init(void);
void uart_tx(char c);
void uart_printstr(const char *str);
void ft_putnbr(int16_t nbr);
#endif
#define UART_H
