/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:55:55 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/11 12:05:55 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

int	main(void)
{
	DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD3);
	
	while (1)
	{
		PORTD |= (1 << DDD5); // red
		_delay_ms(1000);
		PORTD &= ~(1 << DDD5);

		PORTD |= (1 << DDD6); // green
		_delay_ms(1000);
		PORTD &= ~(1 << DDD6);

		PORTD |= (1 << DDD3); // blue
		_delay_ms(1000);
		PORTD &= ~(1 << DDD3);

		PORTD |= (1 << DDD5) | (1 << DDD6); // yellow
        _delay_ms(1000);
		PORTD &= ~(1 << DDD5) & ~(1 << DDD6);

		PORTD |= (1 << DDD6) | (1 << DDD3); // cyan
        _delay_ms(1000);
		PORTD &= ~(1 << DDD6) & ~(1 << DDD3);
        
		PORTD |= (1 << DDD5) | (1 << DDD3); // yellow
        _delay_ms(1000);
		PORTD &= ~(1 << DDD5) & ~(1 << DDD3);

        PORTD |= (1 << DDD5) | (1 << DDD3) | (1 << DDD6); // white
        _delay_ms(1000);
		PORTD &= ~(1 << DDD5) & ~(1 << DDD3) & ~(1 << DDD6);
	}
}