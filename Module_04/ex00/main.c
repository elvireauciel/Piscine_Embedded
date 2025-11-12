/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 16:52:18 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/13 16:22:35 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

volatile int	state = 0;

ISR(INT0_vect)
{
	state = 1;
}

int	main(void)
{
	DDRD &= ~(1 << DDD2); // SW1 (pd2) in input
	DDRB |= (1 << DDB0);  // light as output

	/*The falling edge of INT0 generates an interrupt request.*/
	EICRA |= (1 << ISC01);

	EIMSK |= (1 << INT0); // INT0: External Interrupt Request 0 Enable
	sei();
	while (1)
	{
		if (state == 1)
		{
			PORTB ^= (1 << PB0);
			_delay_ms(275);
			state = 0;
		}
	}
}