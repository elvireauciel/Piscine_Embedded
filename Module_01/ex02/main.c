/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:10:04 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/13 13:27:46 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

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

	/* Clear OC1A on Compare Match, set OC1A at BOTTOM (non-inverting mode) page 113*/
	TCCR1A |= (1 << COM1A1);
	while (1)
	{
	}
}

