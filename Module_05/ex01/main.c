/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:32:23 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/13 14:49:11 by lasablon         ###   ########.fr       */
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
	char		c;
	const int	len = ft_strlen(s);

	int i, j;
	for (i = 0, j = len - 1; i < j; i++, j--)
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

void	uart_printstr(const char *str)
{
	volatile int	i;

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

void	uart_tx(const char c)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1 << UDRE0)))
	{
	}
	/*copy data to be sent*/
	UDR0 = c;
}

void	set_inputs(const int i)
{
	if (i == 0)
	{
		ADMUX &= ~(1 << MUX0);
		ADMUX &= ~(1 << MUX1);
	}
	if (i == 1)
	{
		ADMUX |= (1 << MUX0);
		ADMUX &= ~(1 << MUX1);
	}
	if (i == 2)
	{
		ADMUX &= ~(1 << MUX0);
		ADMUX |= (1 << MUX1);
	}
}

void	init_ADC(void)
{
	DDRC &= ~(1 << PC0) & ~(1 << PC1) & ~(1 << PC2); // RV1, LDR , NTC in input
	ADMUX |= (1 << REFS0);
	// AVCC with external capacitor at AREF pin
	ADMUX |= (1 << ADLAR);                                // use 8bits
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler
	ADCSRA |= (1 << ADEN);                                // Enables the ADC
}

int	main(void)
{
	uint8_t i;
	uint8_t n;
	init_ADC();
	uart_init();
	while (1)
	{
		for (i = 0; i < 3; i++)
		{
			char s[10] = {0};
			set_inputs(i);
			ADCSRA |= (1 << ADSC); // Start conversion
			while ((ADCSRA & (1 << ADSC)))
			{
			};
			n = ADCH;
			itoa_hex(n, s);
			uart_printstr(s);
			if (i != 2)
				uart_printstr(", ");
		}
		uart_printstr("\r\n");
		_delay_ms(20);
	}
}