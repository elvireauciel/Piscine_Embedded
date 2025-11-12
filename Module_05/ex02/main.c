/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:42:04 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/13 16:42:57 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#define FOSC 16000000 // Clock Speed
#define BAUD 115200   // Baud Rate

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

void	ft_putnbr(int16_t nbr)
{
	if (nbr < 0)
	{
		uart_tx('-');
		nbr = -nbr;
	}
	if (nbr < 10)
	{
		uart_tx(nbr + '0');
	}
	else
	{
		ft_putnbr(nbr / 10);
		ft_putnbr(nbr % 10);
	}
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

void	set_inputs(int i)
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
	/*RV1 + LDR (R14)+ NTC (R20) in input*/
	DDRC &= ~(1 << PC0) & ~(1 << PC1) & ~(1 << PC2);
	/* AVCC with external capacitor at AREF pin*/
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << ADLAR); // use 10bits
	ADCSRA |= (1 << ADPS2); // prescaler
	ADCSRA |= (1 << ADEN);  // Enables the ADC
}

int	main(void)
{
	uint16_t n;
	uint8_t i;
	uart_init();
	init_ADC();
	while (1)
	{
		for (i = 0; i < 3; i++)
		{
			set_inputs(i);
			ADCSRA |= (1 << ADSC); // Start conversion
			while ((ADCSRA & (1 << ADSC))){};
			n = ADC;
			ft_putnbr(n);
			if (i != 2)
				uart_printstr(", ");
		}
		uart_printstr("\r\n");
		_delay_ms(20);
	}
}