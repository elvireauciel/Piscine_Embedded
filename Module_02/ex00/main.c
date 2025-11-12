/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:41:56 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/07 15:19:49 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 115200   // Baud Rate

/*
	USART - Universal Synchronous and Asynchronous serial Receiver and Transmitter
	UBRRn = (16000000 / (16 * 115200)) - 1
	UCSR – USART MSPIM Control and Status Register 
	BAUD -  baud is a common unit of measurement of symbol rate, which is one of
	the components that determine the speed of communication over a data channel. 
*/
void	uart_init(void)
{
	unsigned char	ubrr;

	ubrr = FOSC / 8 / BAUD - 1;
	/* Set baud rate. */
	UBRR0L = ubrr;
	/*Enable receiver and transmitter*/
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	/*Set Frame format used by the USART */
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // sets char size to 8 bits
	UCSR0A |= (1 << U2X0);                   // double speed mode
	UCSR0C &= ~(1 << USBS0);                 // select 1 stop bit
}

void	uart_tx(char c)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1 << UDRE0)))
	{
	}
	/*copy data to be sent*/
	UDR0 = c;
}

int	main(void)
{
	uart_init();
	while (1)
	{
		uart_tx('Z');
		_delay_ms(1000);
	}
}
