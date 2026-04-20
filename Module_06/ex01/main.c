#include "i2c.h"
#include "uart.h"
#include <util/delay.h>

#define SENSOR_ADDRESS 0x38
#define TWI_CALIBARTION_MASK 0x71 // Uses mask 0b01110001

void calibration(void) {
  i2c_write(0xbe);
  i2c_write(0x08);
  i2c_write(0x00);
  _delay_ms(10);
}

void trigger_measurement(void) {
  i2c_start();
  i2c_write((SENSOR_ADDRESS << 1) | TW_WRITE);
  i2c_write(0xAC);
  i2c_write(0x33);
  i2c_write(0x00);
  i2c_stop();
}

int main(void) {
  int i = 0;
  unsigned char calibration_bit;
  uint8_t data[7] = {0};

  uart_init();
  _delay_ms(40); // delay required after power-on
  i2c_init();

  // checks calibration enable bit.
  calibration_bit = (TWSR & TWI_CALIBARTION_MASK);
  if (calibration_bit == 0) {
    calibration();
  }

  while (1) {
    trigger_measurement();
    i2c_read(data, SENSOR_ADDRESS);
    for (i = 0; i < 7; i++) {
      uart_print_hex_value(data[i]);
      uart_tx(' ');
    }
    uart_printstr_endl("");
    i2c_stop();
    _delay_ms(1000);
  }
}
