/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:10:32 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/04 11:09:40 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

int	get_bit(int num, int pos)
{
	return ((num & (1 << pos)) >> pos);
}

int	isPrevstateDifferent(int stateL, int prevStateL, int stateR, int prevStateR)
{
	return ((stateL == 1 && prevStateL == 0) || (stateR == 1
			&& prevStateR == 0));
}

int ButtonsPressed()
{
	return((PIND & (1 << PIND2)) == 0) || ((PIND & (1 << PIND4)) == 0);
}

int	main(void)
{
	uint8_t num = 0;
	int prevStateL = 1;
	int prevStateR = 1;
	int stateL = 0;
	int stateR = 0;
	int ports[4] = {PORTB0, PORTB1, PORTB2, PORTB4};
	int i = 0;
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4); //set les leds en output
	DDRD &= ~(1 << DDD2); //set les boutons en input
	DDRD &= ~(1 << DDD4); //set les boutons en input
	while (1)
	{
		for (i = 0; i < 4; i++)
		{
			if (get_bit(num, i))
				PORTB |= (1 << ports[i]);
			else
				PORTB &= ~(1 << ports[i]);
		}
		stateL = (PIND & (1 << PIND2)) == 0;
		stateR = (PIND & (1 << PIND4)) == 0;
		if (ButtonsPressed() && isPrevstateDifferent(stateL, prevStateL, stateR, prevStateR))
		{
			if ((PIND & (1 << PIND2)) == 0)
				num--;
			if ((PIND & (1 << PIND4)) == 0)
				num++;
		}
		prevStateL = stateL;
		prevStateR = stateR;
		_delay_ms(50);
	}
}