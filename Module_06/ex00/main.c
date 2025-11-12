/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:48:11 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/14 14:36:37 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/twi.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 115200   // Baud Rate
#define BDIV (FOSC / 100000 - 16) / 2 + 1

char	*i2c_return_code_desc(uint8_t status_code)
{
	if (status_code == TW_START)
		return ("START acknowledge.");
	else if (status_code == TW_REP_START)
		return ("REPEATED START acknowledge.");
	else if (status_code == TW_MT_SLA_ACK)
		return ("Master Transmitter: Slave ACK");
	else if (status_code == TW_MT_SLA_NACK)
		return ("Master Transmitter : Slave NACK");
	else if (status_code == TW_MT_DATA_ACK)
		return ("Master Transmitter : Data ACK");
	else if (status_code == TW_MT_DATA_NACK)
		return ("Master Transmitter: Data NACK");
	else if (status_code == TW_MR_SLA_ACK)
		return ("Master Receiver : Slave ACK");
	else if (status_code == TW_MR_SLA_NACK)
		return ("Master Receiver : Slave NACK");
	else if (status_code == TW_MR_DATA_ACK)
		return ("Master Receiver : Data ACK");
	else if (status_code == TW_MR_DATA_NACK)
		return ("Master Receiver : Data NACK");
	else if (status_code == TW_MT_ARB_LOST || status_code == TW_MR_ARB_LOST)
		return ("Arbitration Lost");
	else if (status_code == TW_ST_SLA_ACK)
		return ("Slave Transmitter : Slave ACK");
	else if (status_code == TW_ST_ARB_LOST_SLA_ACK)
		return ("Arbitration Lost in SLA+R/W, Slave ACK");
	else if (status_code == TW_ST_DATA_ACK)
		return ("Slave Transmitter : Data ACK");
	else if (status_code == TW_ST_DATA_NACK)
		return ("Slave Transmitter : Data NACK");
	else if (status_code == TW_ST_LAST_DATA)
		return ("Slave Transmitter : Last Data");
	else if (status_code == TW_SR_SLA_ACK)
		return ("Slave Receiver : Slave ACK");
	else if (status_code == TW_SR_ARB_LOST_SLA_ACK)
		return ("Arbitration Lost in SLA+R/W, Slave ACK");
	else if (status_code == TW_SR_GCALL_ACK)
		return ("General Call : Slave ACK");
	else if (status_code == TW_SR_ARB_LOST_GCALL_ACK)
		return ("Arbitration Lost in General Call, Slave ACK");
	else if (status_code == TW_SR_DATA_ACK)
		return ("Slave Receiver : Data ACK");
	else if (status_code == TW_SR_DATA_NACK)
		return ("Slave Receiver : Data NACK");
	else if (status_code == TW_SR_GCALL_DATA_ACK)
		return ("General Call : Data ACK");
	else if (status_code == TW_SR_GCALL_DATA_NACK)
		return ("General Call : Data NACK");
	else if (status_code == TW_SR_STOP)
		return ("Slave Receiver : STOP received");
	else if (status_code == TW_NO_INFO)
		return ("No state information available");
	else if (status_code == TW_BUS_ERROR)
		return ("Bus Error");
	else
		return ("Unknown Status Code");
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

void	i2c_init(void)
{
	TWSR = 0;    // set prescaler at 1
	TWBR = BDIV; // Set bit rate register
}

void	i2c_start(void)
{
	TWCR |= (1 << TWINT); // TWI Interrupt Flag
	TWCR |= (1 << TWSTA); // Set TWI START Condition Bit
	// enables TWI operation and activates the TWI interface
	TWCR |= (1 << TWEN);
	while (!(TWCR & (1 << TWINT))) // Wait for TWINT Flag set.
		;
}

void	i2c_stop(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); //Transmit STOP condition
}
void	uart_tx(char c)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1 << UDRE0)))
		;
	/*copy data to be sent*/
	UDR0 = c;
}

void	ft_putnbr(int16_t nbr)
{
	if (nbr < 0)
	{
		uart_tx('-');
		nbr = -nbr;
	}
	if (nbr < 10)
	{
		uart_tx(nbr + '0');
	}
	else
	{
		ft_putnbr(nbr / 10);
		ft_putnbr(nbr % 10);
	}
}
int	main(void)
{
	char	*return_status;
	uint8_t	sensorAdress;

	sensorAdress = 0x38;
	uart_init();
	i2c_init();
	i2c_start();
	return_status = i2c_return_code_desc(TWSR & 0xf8); // mask 11111000
	
	uart_printstr(return_status);
	uart_printstr("\r\n");
	TWDR = (sensorAdress << 1) | TW_WRITE;	//about to write to sensor Adress
	/*Clear TWINT bit in TWCR to start transmission of address
	enables TWI operation and activates the TWI interface*/
	TWCR = (1 << TWINT) | (1 << TWEN);
	
	while (!(TWCR & (1 << TWINT))) //wait for TW_WRITE to be transmitted
		;
	return_status = i2c_return_code_desc(TWSR & 0xf8);
	uart_printstr(return_status);
	uart_printstr("\r\n");
	i2c_stop();
	while (1)
	{
	}
}
