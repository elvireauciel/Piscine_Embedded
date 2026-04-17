#include "i2c.h"
#include "uart.h"
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

#define SENSOR_ADDRESS 0x38

void write_byte(uint8_t byte) {
  int i;

  for (i = 7; i >= 0; i--) {
    uart_tx(((byte & (1 << i)) >> i) + '0');
  }
}

void write_uint32(uint32_t num) {
  for (uint8_t i = 4; i > 0; i -= 1) {
    write_byte((uint8_t)(num >> ((i - 1) * 8) & 0xFF));
    uart_tx(' ');
  }
}

void trigger_measurement(void) {
  i2c_start();
  i2c_write((SENSOR_ADDRESS << 1) | TW_WRITE);
  i2c_write(0xAC); // trigger measurement 0xAC
  i2c_write(0x33);
  i2c_write(0x00);
  while ((TWSR & (1 << 7))) // waits for measurement completed
    ;
  i2c_stop();
  _delay_ms(80);
}

void calibration(void) {
  i2c_write(0xBE);
  i2c_write(0x08);
  i2c_write(0x00);
  _delay_ms(10);
}

void verify_calibration(void) {
  i2c_start();
  i2c_write((SENSOR_ADDRESS << 1) | TW_WRITE);
  i2c_write(0x71);
  if ((TWSR & 0x18) != 0x18) // checks calibration enable bit
    calibration();
  i2c_stop();
}

double calculate_humidity(uint8_t *data) {
  uint32_t humidity;
  // uart_print_hex_value((uint32_t)data[1]);
  // uart_printstr(" ");
  // uart_print_hex_value(((uint32_t)data[2]));
  // uart_printstr(" ");
  // uart_print_hex_value(((uint32_t)data[3]) >> 4);
  // uart_printstr(" ");
  // uart_printstr("\r\n");

  humidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) |
             ((uint32_t)data[3] >> 4);
  float denominator = 1048576.0f;
  return ((humidity / denominator) * 100.0f);
}

double calculate_temp(uint8_t *data) {
  uint32_t temp = 0;

  temp += (uint32_t)(data[3] & 0x0F) << 16;
  temp += (uint32_t)(data[4]) << 8;
  temp += (uint32_t)(data[5]);

  return (((temp / 1048576.0) * 200.0) - 50.0);
}

int main(void) {
  uint8_t data[7] = {0};
  char temp[10] = {0};
  char humidity[10] = {0};

  uart_init();
  i2c_init();
  verify_calibration();

  while (1) {
    trigger_measurement();
    i2c_start();
    i2c_write((SENSOR_ADDRESS << 1) | TW_READ);

    for (int i = 0; i < 7; i++) {
      i2c_read();
      data[i] = TWDR;
      // uart_print_hex_value(data[i]);
      // uart_printstr(" ");
    }

    uart_printstr_endl("");
    i2c_stop();

    dtostrf(calculate_humidity(data), 4, 2, humidity);
    uart_printstr("Humidity: ");
    uart_printstr(humidity);
    uart_printstr_endl("%");

    dtostrf(calculate_temp(data), 4, 2, temp);
    uart_printstr("Temperature: ");
    uart_printstr(temp);
    uart_printstr_endl("C");

    _delay_ms(2000);
  }
}
