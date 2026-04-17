#ifndef I2C_H
#define I2C_H
#include <avr/io.h>
#include <util/twi.h>

#define BDIV (((FOSC) / 100000UL - 16) / 2 + 1)
#define TWI_STATUS_MASK 0xf8

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
const char *i2c_get_status_description(uint8_t status_code);
uint8_t i2c_get_status(void);
#endif // I2C_H
