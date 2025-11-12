/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:36:37 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/13 16:18:57 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#define INCREMENT 1
#define DECREMENT 0
#define HIGH 0
#define LOW 1
#define PRESCALER 256

volatile uint8_t	num = 0;

volatile int		stateL = HIGH;
volatile int		stateR = HIGH;
volatile int		oldStateL = LOW;
volatile int		oldStateR = LOW;

volatile int		bouncingTime;
int	get_bit(int num, int pos)
{
	return ((num & (1 << pos)) >> pos);
}

void	light_leds(void)
{
	int	i;
	int	ports[4] = {PORTB0, PORTB1, PORTB2, PORTB4};

	i = 0;
	for (i = 0; i < 4; i++)
	{
		if (get_bit(num, i))
			PORTB |= (1 << ports[i]);
		else
			PORTB &= ~(1 << ports[i]);
	}
}

void	routine(int status)
{
	if (status == DECREMENT)
		num--;
	if (status == INCREMENT)
		num++;
	_delay_ms(50);
	light_leds();
}

ISR(INT0_vect)
{
	stateL = (PIND & (1 << PIND2)) ? HIGH : LOW;
	if (stateL == HIGH && oldStateL == LOW)
	{
		routine(INCREMENT);
		_delay_ms(50);
	}
	oldStateL = stateL;
}

ISR(PCINT2_vect)
{
	stateR = (PIND & (1 << PIND4)) ? HIGH : LOW;
	if (stateR == HIGH && oldStateR == LOW)
	{
		routine(DECREMENT);
		_delay_ms(50);
	}
	oldStateR = stateR;
}

int	main(void)
{
	/*	set les leds en output	*/
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
	
	/*set les boutons SW1 et SW2 en input*/
	DDRD &= ~(1 << DDD2);
	DDRD &= ~(1 << DDD4);

	/*	The SW2 interrupt is accessible only using its pin and i cannot change its
		Interrupt sens controll, it will always be on any logical change	*/
	PCICR |= (1 << PCIE2);    // PCIE2: Pin Change Interrupt Enable 2
	PCMSK2 |= (1 << PCINT20); // Enable interrupt for pin 20 (SW1)

	/*	I need to set the SW1 in any logical level interrupt like the SW2	*/
	EIMSK |= (1 << INT0);  // INT0: External Interrupt Request 0 Enable
	EICRA |= (1 << ISC00);// Any logical change on INT0 generates an interrupt request.

	sei(); // enable global interrupts
	while (1)
	{
	}
}