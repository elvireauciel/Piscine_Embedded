/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:02:47 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/04 11:09:30 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

/*

DDRB = Data Direction Register B : bits a set pour traiter les ports comme input/output
PIND = Port D Input Pins Register : registre read only qui me permet de lire dans le port D
SOURCE: https://docs.arduino.cc/retired/hacking/software/PortManipulation/

*/
int	main(void)
{
	DDRD &= ~(1 << DDB2);  // set le bit de DDB2 comme output
	DDRB = DDRB | (1 << DDB0); // set le bit de DDB0 comme output
	while (1)
	{
		if ((PIND & (1 << PIND2)) == 0)	// si PIND2 est low c'est que le bouton est appuye
		{
			PORTB = PORTB | (1 << PORTB0);// set le bit de PORTB0 a 1 pour allumer la led
		}
		else
			PORTB &= ~(1 << PORTB0);
	}
}