/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:30:49 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/07 15:46:42 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 115200   // Baud Rate

static int	ft_islower(int c)
{
	if ((c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}

int	ft_toupper(int c)
{
	if (ft_islower(c))
		c -= 32;
	return (c);
}

static int	ft_isupper(int c)
{
	if ((c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	ft_tolower(int c)
{
	if (ft_isupper(c))
		c += 32;
	return (c);
}

void	uart_init(void)
{
	unsigned char	ubrr;

	ubrr = FOSC / 8 / BAUD - 1;
	/* Set baud rate. */
	UBRR0L = ubrr;
	/*Enable receiver and transmitter*/
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0B |= (1 << RXCIE0);
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

ISR(USART_RX_vect)
{
	char	c;
	c = UDR0; //reads the content of the buffer
    
	if (ft_isupper(c))
		c = ft_tolower(c);
	else if (ft_islower(c))
		c = ft_toupper(c);
	uart_tx(c); //send the reverse case value
}

int	main(void)
{
	uart_init();
	sei(); //enable global interrupts
	while (1)
	{
	}
}