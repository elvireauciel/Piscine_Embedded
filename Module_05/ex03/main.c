/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:02:00 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/17 13:33:57 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#define FOSC 16000000 // Clock Speed
#define BAUD 115200   // Baud Rate

/*
	T = { [(ADCH << 8) | ADCL] - TOS } / k
	where ADCn are the ADC data registers, k is a fixed coefficient and
	TOS is the temperature sensor offset value
	determined and stored into EEPROM as a part of the production test.
*/

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

void	ft_putnbr(uint16_t n)
{
	if (n < 10)
	{
		uart_tx(n + '0');
	}
	else
	{
		ft_putnbr(n / 10);
		ft_putnbr(n % 10);
	}
}

uint16_t	calculate_Ctemp(void)
{
	//Table 24-2. Temperature vs. Sensor Output Voltage (Typical Case)
	return ((ADC * 25) / 314);
}

void	init_ADC(void)
{
	ADMUX |= (1 << REFS0) | (1 << REFS1);                 // Internal 1.1V
	ADMUX |= (1 << MUX3);                                 // ADC8 (Temp sensor)
	ADCSRA |= (1 << ADPS1) | (1 << ADPS2) | (1 << ADPS0); // prescaler
	ADCSRA |= (1 << ADEN);                                // Enables the ADC
}

int	main(void)
{
	uart_init();
	init_ADC();
	uint16_t Ctemp;
	while (1)
	{
		ADCSRA |= (1 << ADSC);
		while ((ADCSRA & (1 << ADSC))){}
		Ctemp = calculate_Ctemp();
		ft_putnbr(Ctemp);
		uart_printstr("\r\n");
		_delay_ms(20);
	}
}