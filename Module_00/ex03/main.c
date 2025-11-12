/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:02:47 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/04 11:09:51 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

int	main(void)
{
	DDRD &= ~(1 << DDD2); // set PORTD en input
	DDRB = DDRB | (1 << DDB0); // set PORTB0 comme output
	int prevState = 1;
	while (1)
	{
		int state = (PIND & (1 << PIND2)) == 0;
		if (state == 1 && prevState == 0) // si PORTD2 est low
		{
			PORTB ^= (1 << PORTB0);// set le bit de PORTB0 a l'inverse de son etat actuel
		}
		prevState = state;
		_delay_ms(50);
	}
}