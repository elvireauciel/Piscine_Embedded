#include "i2c.h"
#include <stdint.h>
// Initialize I2C (TWI) hardware
void i2c_init(void) {
  TWSR = 0;    // Set prescaler to 1 (no division)
  TWBR = BDIV; // Set bit rate (defines SCL clock frequency)
}

// Send START condition on I2C bus
void i2c_start(void) {
  TWCR |= (1 << TWINT); // Clear interrupt flag to start operation
  TWCR |= (1 << TWSTA); // Set START condition
  TWCR |= (1 << TWEN);  // Enable TWI hardware

  while (!(TWCR & (1 << TWINT))) // Wait until START is transmitted
    ;
}

// Send STOP condition on I2C bus
void i2c_stop(void) {
  // TWSTO will be cleared automatically after STOP is sent
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

// Return current TWI status (masked)
uint8_t i2c_get_status(void) {
  return TWSR & TWI_STATUS_MASK; // Mask prescaler bits
}

// Wait until current transmission is complete
void i2c_wait_transmission(void) {
  while (!(TWCR & (1 << TWINT))) // TWINT set = operation complete
    ;
}

// Print human-readable status over UART
void i2c_print_status(uint8_t status) {
  const char *return_status;

  return_status = i2c_get_status_description(status);
  uart_printstr(return_status);
  uart_printstr("\r\n");
}

// Read 7 bytes from a slave device
void i2c_read(uint8_t *data, uint8_t sensor_address) {
  i2c_start(); // Send START condition

  // Send slave address with READ bit (LSB = 1)
  i2c_write((sensor_address << 1) | TW_READ);

  for (int i = 0; i < 7; i++) {
    if (i == 6) {
      // Enable TWI and send NACK after receiving byte
      TWCR = (1 << TWINT) | (1 << TWEN);
    } else {
      // Enable TWI and send ACK after receiving byte
      TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    }

    i2c_wait_transmission(); // Wait for reception to complete

    data[i] = TWDR; // Read received byte
  }
}

// Write one byte on I2C bus
void i2c_write(unsigned char data) {
  TWDR = data; // Load data into data register

  // Start transmission
  TWCR = (1 << TWINT) | (1 << TWEN);

  i2c_wait_transmission(); // Wait until sent
}

// Convert TWI status code to readable string
const char *i2c_get_status_description(uint8_t status_code) {

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
