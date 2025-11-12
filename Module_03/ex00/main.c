/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:20:52 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/08 11:16:36 by lasablon         ###   ########.fr       */
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
		PORTD |= (1 << DDD5); //r
		_delay_ms(1000);
		PORTD &= ~(1 << DDD5);

		PORTD |= (1 << DDD6); //g
		_delay_ms(1000);
		PORTD &= ~(1 << DDD6);

		PORTD |= (1 << DDD3); //b
		_delay_ms(1000);
		PORTD &= ~(1 << DDD3);
	}
}