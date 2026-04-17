#include "i2c.h"
#include "uart.h"
#include <avr/io.h>

#define SENSOR_ADDRESS 0x38

int main(void) {
  uart_init(); // initialize uart for output purposes

  i2c_init();  // initialise i2c comunication (setting bit rate)
  i2c_start(); // send START condition on the I2C bus
  uart_printstr_endl(i2c_get_status_description(TWSR & TWI_STATUS_MASK));

  // Load slave address + write bit into data register
  TWDR = (SENSOR_ADDRESS << 1) | TW_WRITE;
  i2c_start(); // send START condition on the I2C bus
  uart_printstr_endl(i2c_get_status_description(TWSR & TWI_STATUS_MASK));
  // Send STOP condition to release the bus
  i2c_stop();
  while (1) {
  }
}
