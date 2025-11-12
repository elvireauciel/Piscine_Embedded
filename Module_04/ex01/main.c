/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 16:52:18 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/13 16:23:55 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#define PRESCALER 256
#define FOSC 16000000 // Clock Speed
#define BAUD 115200   // Baud Rate

volatile int	ascending = 1;

ISR(TIMER0_COMPA_vect)
{
	if (ascending)
		OCR1A += 1;
	else
		OCR1A -= 1;
	if (OCR1A == 0x00FF)
		ascending = 0;
	if (OCR1A == 0)
		ascending = 1;
}

void	init_timer(void)
{
	DDRB |= (1 << DD1); // led in output
	/*  TIMER 1: LIGHT THE LED !!  */
	/*value to be incremented every ms*/
	OCR1A = 0;
	/* mode 256 (prescaler)*/
	TCCR1B |= (1 << CS12);
	/*Fast PWM, 8-bit for the timer 1*/
	TCCR1A |= (1 << WGM10);
	TCCR1B |= (1 << WGM12);
	/*Clear OC0A on Compare Match, set OC0A at BOTTOM, (non-inverting mode)*/
	TCCR1A |= (1 << COM0A1);
	
	/*  TIMER 0: INTENSITY!!  */
	/*I want the interrupt to be called 510times in a ms*/
	OCR0A = F_CPU / PRESCALER / 510;
	/*mode 256 (prescaler)*/
	TCCR0B |= (1 << CS02);
	/*CTC Mode Enable*/
	TCCR0A |= (1 << WGM01);
	/*Timer/Counter0 Output Compare Match A Interrupt Enable*/
	TIMSK0 |= (1 << OCIE0A);
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

int	main(void)
{
	uart_init();
	init_timer();
	sei();
	while (1)
	{
	}
}
