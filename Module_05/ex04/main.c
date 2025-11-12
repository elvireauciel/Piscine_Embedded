/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:06:26 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/13 16:39:38 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#define FOSC 16000000 // Clock Speed
#define BAUD 115200   // Baud Rate
#define RED PB5
#define GREEN PB6
#define BLUE PB3

void	set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

void	init_rgb(void)
{
	DDRD |= (1 << RED) | (1 << GREEN) | (1 << BLUE);
}

void	wheel(uint8_t pos)
{
	pos = 255 - pos;
	if (pos < 85)
	{
		set_rgb(255 - pos * 3, 0, pos * 3);
	}
	else if (pos < 170)
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	}
	else
	{
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

void	init_timer(void)
{
	/*prescaler to 0 for both timers*/
	TCCR0B |= (1 << CS00);
	TCCR2B |= (1 << CS20);
	/* set fast pwm mode for both timers*/
	TCCR0A |= (1 << WGM00) | (1 << WGM01);
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
	// Clear OC0A/OC0B on Compare Match, set OC0A/OC0B at BOTTOM
	TCCR2A |= (1 << COM2B1); // Clear OC2B on Compare Match,set OC2B at BOTTOM
}

void	init_ADC(void)
{
	DDRC &= ~(1 << PC0); // RV1 in input
	ADMUX |= (1 << REFS0);
	// AVCC with external capacitor at AREF pin
	ADMUX |= (1 << ADLAR);                                // use 8bits
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); // prescaler
	ADCSRA |= (1 << ADEN);                                // Enables the ADC
}

void	light_leds(uint8_t n)
{

    if (n == 0)
        PORTB &= ~(1 << PB0) & ~(1 << PB1) & ~(1 << PB2) & ~(1 << PB4);
	if (n >= 64 && n < 128)
    {
        PORTB |= (1 << PB0) ;
        PORTB &= ~(1 << PB1) & ~(1 << PB2) & ~(1 << PB4);
    }
	else if (n >= 128 && n < 192)
    {
        PORTB |= (1 << PB0) |(1 << PB1);
        PORTB &= ~(1 << PB2) & ~(1 << PB4);
    }
	else if (n >= 192 && n < 255)
    {
        PORTB |= (1 << PB0) |(1 << PB1)| (1 << PB2) ;
        PORTB &= ~(1 << PB4);
    }
	if (n == 255)
		PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
}

int	main(void)
{
	uint8_t n;
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);

	init_ADC();
	init_timer();
	init_rgb();
	while (1)
	{
		ADCSRA |= (1 << ADSC); // Start conversion
		while ((ADCSRA & (1 << ADSC))){}
		n = ADCH;
		wheel(n);
        light_leds(n);
		_delay_ms(100);
	}
}