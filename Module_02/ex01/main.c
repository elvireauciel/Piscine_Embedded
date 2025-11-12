/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:58:47 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/07 15:37:52 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 115200   // Baud Rate

/*
	USART - Universal Synchronous and Asynchronous serial Receiver and Transmitter
	UBRRn = (16000000 / (8 * 115200)) - 1
	UCSR – USART MSPIM Control and Status Register 
	BAUD -  baud is a common unit of measurement of symbol rate, which is one of
	the components that determine the speed of communication over a data channel. 
	Time period = (1 + Match Value) / (Clock Speed / Prescaler) 
	Time period = (1 + 31250) / (16000000 / 1024) = 31251 / 15625 = 2 seconds
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

void	uart_printstr(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		/* Wait for empty transmit buffer */
		while (!(UCSR0A & (1 << UDRE0))){}
		/*copy data to be sent*/
		UDR0 = str[i];
		i++;
	}
}

ISR(TIMER1_COMPA_vect) // calls this routine when compare match
{
    uart_printstr("Hello World!\n\r");
}

void timer_init()
{
	OCR1A = 31250; // set the match value to 31250

	TCCR1B |= (1 << WGM12); // Toggle mode CTC (Clear Timer on Compare)

	TCCR1B |= (1 << CS10);  // mode 1024 1 (prescaler)
	TCCR1B &= ~(1 << CS11); // mode 1024 0 (prescaler)
	TCCR1B |= (1 << CS12);  // mode 1024 1 (prescaler)
	
	/* TIMSK1 – Timer/Counter1 Interrupt Mask Register*/
    TIMSK1 |= (1 << OCIE1A); // Enables Output Compare A Match Interrupt 
    
}

int	main(void)
{
	uart_init();
    sei(); // enable interrupts globaly
    timer_init();
	while (1)
	{
	}
}