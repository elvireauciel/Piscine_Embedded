/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:02:47 by lasablon          #+#    #+#             */
/*   Updated: 2025/06/17 17:52:46 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

/*
DDRB = Data Direction Register B : bits a set pour traiter les ports comme input/output
PORTB = Port B Data Register : registre
*/
int	main(void)
{
	// La led D1 est liee au PORTB0
	DDRB = DDRB | (1 << DDB0);// set le bit de DDB0 comme output
	PORTB = PORTB | (1 << PORTB0);// set le bit de PORTB0 a 1 pour allumer la led
	
	while(1){}
}