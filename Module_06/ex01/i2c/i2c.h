#ifndef I2C_H
#define I2C_H

#include "uart.h"
#include <avr/io.h>
#include <util/twi.h>

#define BDIV (((FOSC) / 100000UL - 16) / 2 + 1)
#define TWI_STATUS_MASK 0xf8

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
const char *i2c_get_status_description(uint8_t status_code);
uint8_t i2c_get_status(void);
void i2c_print_status(uint8_t status);
void i2c_wait_transmission(void);
void i2c_read(uint8_t *data, uint8_t sensor_address);
void i2c_write(unsigned char data);
#endif // I2C_H
