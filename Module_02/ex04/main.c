/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 09:50:51 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/07 16:32:00 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 115200   // Baud Rate

int	ft_strncmp(const char *str1, const char *str2, uint16_t n)
{
	uint16_t		i;
	unsigned char	*s1;
	unsigned char	*s2;

	i = 0;
	s1 = (unsigned char *)str1;
	s2 = (unsigned char *)str2;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

int	ft_isprint(int c)
{
	if (c >= ' ' && c <= '~')
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

char	uart_rx(void)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1 << RXC0)))
	{
	};
	/* Get and return received data from buffer */
	return (UDR0);
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

void	uart_printstr(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		while (!(UCSR0A & (1 << UDRE0)))
		{
		}
		/*copy data to be sent*/
		UDR0 = str[i];
		i++;
	}
}

void	handle_answer(char *str, int is_password)
{
	int		i;
	char	c;

	i = 0;
	while (c != '\r' && i < 100)
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
		else if (is_password)
			uart_tx('*');
		else
			uart_tx(c);
		if (ft_isprint(c) && c != 127)
			str[i++] = c; // if the char is printable add it to the str
	}
	str[i] = '\0';
	uart_printstr("\n\r");
}

void	light_routine(void)
{
	int	lights[4] = {PORTB0, PORTB1, PORTB2, PORTB4};
	int	i;

	i = 0;
	while (1)
	{
		PORTB ^= (1 << DDB0) | (1 << DDB2);
		_delay_ms(300);
		PORTB ^= (1 << DDB1) | (1 << DDB4);
		PORTB ^= (1 << DDB0) | (1 << DDB2);
		_delay_ms(300);
		PORTB ^= (1 << DDB1) | (1 << DDB4);
		for (i = 0; i < 4; i++)
		{
			PORTB ^= (1 << lights[i]);
			_delay_ms(150);
		}
		for (i = 0; i < 4; i++)
		{
			PORTB &= ~(1 << lights[i]);
		}
		for (i = 3; i >= 0; i--)
		{
			PORTB |= (1 << lights[i]);
			_delay_ms(150);
			PORTB &= ~(1 << lights[i]);
		}
		_delay_ms(150);
	}
}

int	main(void)
{
	char const *password = "12345";
	char const *username = "elvire";
	char r_password[100] = {0};
	char r_username[100] = {0};
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
	uart_init();

	while (ft_strncmp(username, r_username, 6) != 0 || ft_strncmp(password,
			r_password, 7) != 0)
	{
		uart_printstr("Enter your login:\n\r\tusername: ");
		handle_answer(r_username, 0);
		uart_printstr("\tpassword: ");
		handle_answer(r_password, 1);
		if (ft_strncmp(username, r_username, 6) != 0 || ft_strncmp(password,
				r_password, 7) != 0)
			uart_printstr("Bad username/password combinaison\n\r");
	}
	uart_printstr("Hello ");
	uart_printstr(username);
	uart_printstr("!\n\r");
	uart_printstr("Shall we play a game?\n\rChange your password btw...\n\r");
	light_routine();
}