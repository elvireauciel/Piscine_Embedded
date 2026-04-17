#include "i2c.h"

void i2c_init(void) {
  TWSR = 0;    // set prescaler at 1
  TWBR = BDIV; // Set bit rate register
}

void i2c_start(void) {
  //  initiate transmission of START
  TWCR |= (1 << TWINT); // TWI Interrupt Flag
  TWCR |= (1 << TWSTA); // Set TWI START Condition Bit
  // enables TWI operation and activates the TWI interface
  TWCR |= (1 << TWEN);
  while (!(TWCR & (1 << TWINT))) // Wait for TWINT Flag set
    ;
}

void i2c_stop(void) {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Transmit STOP condition
}
uint8_t i2c_get_status(void) { return TWSR & TWI_STATUS_MASK; }

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
