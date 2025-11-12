/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:50:54 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/07 13:03:03 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

int	isPrevstateDifferent(int stateL, int prevStateL, int stateR, int prevStateR)
{
	return ((stateL == 1 && prevStateL == 0) || (stateR == 1
			&& prevStateR == 0));
}

int	ButtonsPressed(void)
{
	return ((PIND & (1 << PIND2)) == 0) || ((PIND & (1 << PIND4)) == 0);
}

void	handle_buttons(void)
{
	int	prevStateL;
	int	prevStateR;
	int	stateL;
	int	stateR;

	prevStateL = 1;
	prevStateR = 1;
	stateL = 0;
	stateR = 0;
	DDRD &= ~(1 << DDD2); // set les boutons en input
	DDRD &= ~(1 << DDD4); // set les boutons en input
	while (1)
	{
		stateL = (PIND & (1 << PIND2)) == 0;
		stateR = (PIND & (1 << PIND4)) == 0;
		if (ButtonsPressed() && isPrevstateDifferent(stateL, prevStateL, stateR, prevStateR))
		{
			if ((PIND & (1 << PIND2)) == 0)
			{
				if (OCR1A < 62500)
					OCR1A += 6250; // increment bottom value
			}
			if ((PIND & (1 << PIND4)) == 0)
			{
				if (OCR1A > 6250)
					OCR1A -= 6250; // decrement bottom value
			}
		}
		prevStateL = stateL;
		prevStateR = stateR;
		_delay_ms(20);
	}
}

int	main(void)
{
	DDRB |= (1 << DDB1); // set led as output
	OCR1A = 6250;        // bottom value
	ICR1 = 62500;        // top value

	TCCR1B &= ~(1 << CS10); // mode 256 0 (prescaler) (page 117)
	TCCR1B &= ~(1 << CS11); // mode 256 0 (prescaler)
	TCCR1B |= (1 << CS12);  // mode 256 1 (prescaler)

	// Mode Fast PWM with ICR1 at top (page 141)
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	TCCR1A |= (1 << WGM11);

	/* Clear OC1A on Compare Match, set OC1A at BOTTOM (non-inverting mode) (page 113)*/
	TCCR1A |= (1 << COM1A1);
	while (1)
	{
		handle_buttons();
	}
}