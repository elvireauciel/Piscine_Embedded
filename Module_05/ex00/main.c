/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:25:32 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/13 16:28:52 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#define FOSC 16000000 // Clock Speed
#define BAUD 115200   // Baud Rate

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	reverse(char *s)
{
	char	c;

	int i, j;
	for (i = 0, j = ft_strlen(s) - 1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void	itoa_hex(int n, char *s)
{
	char	*base;
	int		i;

	i = 0;
	base = "0123456789abcdef";
	if (n == 0)
		s[i++] = '0';
	while (n > 0)
	{
		s[i] = base[n % 16];
		n /= 16;
		i++;
	}
	s[i] = '\0';
	reverse(s);
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

char	uart_rx(void)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1 << RXC0)))
		;
	/* Get and return received data from buffer */
	return (UDR0);
}

void	uart_printstr(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		while (!(UCSR0A & (1 << UDRE0)))
		{
		}
		/*copy data to be sent*/
		UDR0 = str[i];
		i++;
	}
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
	uint16_t n;
	DDRC &= ~(1 << PC0);   // PC0 in input
	ADMUX |= (1 << REFS0); // AVCC with external capacitor at AREF pin
	ADMUX |= (1 << ADLAR); // use 8bits
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); //prescaler
	ADCSRA |= (1 << ADEN); //Enables the ADC
	uart_init();
	while (1)
	{
		char s[50] = {0};
		ADCSRA |= (1 << ADSC); // Start conversion
		while (ADCSRA & (1 << ADSC))
		{
		}
		n = ADCH;
		itoa_hex(n, s);
		uart_printstr(s);
		uart_printstr("\r\n");
		_delay_ms(20);
	}
}
