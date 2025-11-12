/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 10:33:14 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/07 13:00:24 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

int	main(void)
{
	DDRB |= (1 << DDB1); // met D2 en output

	uint16_t i = 0;
	uint16_t j = 0;
	while (1)
	{
		i = 0;
		while (i < 1412) // valeur un peu au pif mais ca fonctionne
		{
			j = 0;
			while (j < 1412)  // valeur un peu au pif mais ca fonctionne
				j++;
			i++;
		}
		PORTB ^= (1 << PORTB1); // allume/eteinds D2
	}
}