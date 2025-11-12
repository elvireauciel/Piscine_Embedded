/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:29:25 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/10 14:23:02 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define RED DDD5
#define GREEN DDD6
#define BLUE DDD3

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
	
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1); // Clear OC0A/OC0B on Compare Match, set OC0A/OC0B at BOTTOM
	TCCR2A |= (1 << COM2B1);                 // Clear OC2B on Compare Match, set OC2B at BOTTOM
}

int	main(void)
{
	uint8_t	i = 0;

	init_rgb();
	init_timer();
	while (1)
	{
		wheel(i);
		_delay_ms(50);
		i++;
	}
}
