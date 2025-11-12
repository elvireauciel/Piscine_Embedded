/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:20:48 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/11 12:20:27 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define RED DDD5
#define GREEN DDD6
#define BLUE DDD3
#define FOSC 16000000 // Clock Speed
#define BAUD 115200   // Baud Rate

int	ft_isprint(int c)
{
	if (c >= ' ' && c <= '~')
		return (1);
	return (0);
}

int	ft_isdigit(int c)
{
	if ((c >= '0' && c <= '9'))
		return (1);
	return (0);
}

int	ft_is_hex(int c)
{
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))
		return (1);
	return (0);
}

void	uart_init(void)
{
	unsigned char	ubrr;

	ubrr = FOSC / 8 / BAUD - 1;
	/* Set baud rate. */
	UBRR0L = ubrr;
	/*Enable receiver and transmitter*/
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	/*Set Frame format used by the USART */
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // sets char size to 8 bits
	UCSR0A |= (1 << U2X0);                   // double speed mode
	UCSR0C &= ~(1 << USBS0);                 // select 1 stop bit
}

void	uart_tx(char c)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1 << UDRE0)))
	{
	}
	/*copy data to be sent*/
	UDR0 = c;
}

char	uart_rx(void)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1 << RXC0)))
		;
	/* Get and return received data from buffer */
	return (UDR0);
}
void	uart_printstr(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		uart_tx(str[i]);
		i++;
	}
}

void	init_rgb(void)
{
	DDRD |= (1 << RED) | (1 << GREEN) | (1 << BLUE);
}

void	set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	// set bottom values
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

void	get_input(char *str)
{
	int		i;
	char	c;

	c = 0;
	i = 0;
	while (i < 50 && c != '\r')
	{
		c = uart_rx();
		if (c == '\r') // if enter key was pressed
			break ;
		if (c == 127) // if backspace key was pressed
		{
			if (i != 0) // protection to not overwrite on previous characters
			{
				uart_tx(8);   // go to previous letter
				uart_tx(' '); // replace it with space
				uart_tx(8);   // move cursor to the left again
				i--;
			}
		}
		else if (ft_isprint(c))
			uart_tx(c);
		if (ft_isprint(c) && c != 127)
			str[i++] = c;
	}
	str[i] = '\0';
	uart_printstr("\r\n");
}

int	ft_in_base(char c, char *base)
{
	int	i;

	i = 0;
	while (base[i])
	{
		if (c == base[i])
			return (i);
		i++;
	}
	return (-1);
}

int	ft_hex_to_int(const char *str)
{
	int		i;
	int		min;
	int		res;
	char	*base;
	int		base_index;

	base = "0123456789ABCDEF";
	i = 0;
	min = 1;
	res = 0;
	while (str[i])
	{
		base_index = ft_in_base(str[i], base);
		res = res * 16 + base_index;
		i++;
	}
	return (res * min);
}

void	split_rgb(char *r, char *g, char *b, char *str)
{
	r[0] = str[1];
	r[1] = str[2];
	r[2] = '\0';
	g[0] = str[3];
	g[1] = str[4];
	g[2] = '\0';
	b[0] = str[5];
	b[1] = str[6];
	b[2] = '\0';
}

int	parse_answer(char *str)
{
	int		i;
	char	r[3];
	char	g[3];
	char	b[3];

	i = 1;
	while (str[i])
	{
		if (!ft_is_hex(str[i]) && str[i] != '#') // is everything in hex
			return (0);
		i++;
	}
	if (i != 7) // is it 7 characters
		return (0);
	if (str[0] != '#') // does it start with #
		return (0);
	split_rgb(r, g, b, str);
	set_rgb(ft_hex_to_int(r), ft_hex_to_int(g), ft_hex_to_int(b));
	return (1);
}

void	init_timer(void)
{
	/*Need to use too  diffrent timers
		because the blue led is onn OC2 and not OC0*/
	/*no prescaler*/
	TCCR0B |= (1 << CS00);
	TCCR2B |= (1 << CS20);
	/* set fast pwm mode (mode 3) , top at 255*/
	TCCR0A |= (1 << WGM00) | (1 << WGM01);
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	// Clear OC0A/OC0B on Compare Match, set OC0A/OC0B at BOTTOM
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
	// Clear OC2B on Compare Match,set OC2B at BOTTOM
	TCCR2A |= (1 << COM2B1);
}

int	main(void)
{
	uart_init();
	init_timer();
	init_rgb();
	char input[50] = {0};

	while (1)
	{
		uart_printstr("Please choose an hexadecimal color : ");
		get_input(input);
		uart_printstr("\r\n");
		while (!parse_answer(input))
		{
			uart_printstr("Chose invalid color : ");
			uart_printstr(input);
			uart_printstr("\r\nPlease put a valid hexadecimal number : ");
			get_input(input);
		}
		uart_printstr("Changed to ");
		uart_printstr(input);
		uart_printstr("\r\n");
	}
}