/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 11:01:58 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/07 12:49:11 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

/*
 * L'horloge est de 16MHz
 * Une impulsion == (16000000 / 1) : 62,5 nano secondes
 * Il faut compter 16000000Hz ce qui est trop grand pour un registre 16bits
 * On divise donc 16MHz par 256 (62ns * 256 == 16µs)
 * 0.5s / 16µs (500000µs / 16µs) - 1 == 31249
 *
 * OCR1A :  Output Compare Register 1 A : value always comares to TCNT1
 * TCCR1 – Timer/Counter1 Control Register : 16bits register
 */

int	main(void)
{
	DDRB |= (1 << DDB1); // led D2 as output

	OCR1A = 31250; // set the match value to 31250

	TCCR1B |= (1 << WGM12); // Toggle mode CTC (Clear Timer on Compare) (page 141)

	/* mode 256 prescaler (page 117) */
	TCCR1B &= ~(1 << CS10);
	TCCR1B &= ~(1 << CS11);
	TCCR1B |= (1 << CS12);

	/* toggle OC1A pin(linked to the led) on compare match  (page 113)*/
	TCCR1A |= (1 << COM1A0);
}