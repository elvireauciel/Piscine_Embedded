/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:38:19 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/17 15:31:13 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#define FOSC 16000000                     // Clock Speed
#define BAUD 115200                       // Baud Rate
#define BDIV (FOSC / 100000 - 16) / 2 + 1 // 22.5.2 Bit Rate Generator Unit
#define SENSOR_ADDRESS 0x38

char *i2c_return_code_desc(uint8_t status_code) {
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
void i2c_wait_transmission(void) {
  while (!(TWCR & (1 << TWINT)))
    ;
}
void i2c_print_status(void) {
  char *return_status;

  return_status = i2c_return_code_desc(TWSR & 0xf8); // mask 11111000
  uart_printstr(return_status);
  uart_printstr("\r\n");
}

void i2c_init(void) {
  TWSR = 0;    // set prescaler at 1
  TWBR = BDIV; // Set bit rate register
}

void i2c_start(void) {
  TWCR |= (1 << TWINT); // TWI Interrupt Flag
  TWCR |= (1 << TWSTA); // Set TWI START Condition Bit
  // enables TWI operation and activates the TWI interface
  TWCR |= (1 << TWEN);
  i2c_wait_transmission();
}

void i2c_stop(void) {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Transmit STOP condition
}

void i2c_read(void) {
  TWCR = (1 << TWINT) | (1 << TWEN);
  i2c_wait_transmission();
}

void i2c_write(unsigned char data) {
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  i2c_wait_transmission();
}

void calibration(void) {
  i2c_write(0xbe);
  i2c_write(0x08);
  i2c_write(0x00);
  _delay_ms(10);
}

void trigger_measurement(void) {
  i2c_start();
  i2c_write((SENSOR_ADDRESS << 1) | TW_WRITE);
  i2c_write(0xAC); // trigger measurement 0xAC
  i2c_write(0x33);
  i2c_write(0x00);
  i2c_stop();
}

int main(void) {
  int i;
  unsigned char calibration_bit;

  i = 0;
  uart_init();
  _delay_ms(40); // delay required after power-on
  i2c_init();

  // checks calibration enable bit. Uses mask 0b01110001
  calibration_bit = (TWSR & 0x71);
  ft_putnbr(calibration_bit);
  if (calibration_bit == 0) {
    calibration(); // proceeds to calbration
  }
  while (1) {
    trigger_measurement();
    i2c_start();
    _delay_ms(80);
    while ((TWSR & (1 << 7))) // waits for measurement completed
      ;
    i2c_write((SENSOR_ADDRESS << 1) | TW_READ);
    for (i = 0; i < 7; i++) {
      i2c_read();
      print_hex_value(TWDR >> 1); // remove last bit
      uart_tx(' ');
    }
    uart_printstr("\r\n");
    i2c_stop();
  }
}
