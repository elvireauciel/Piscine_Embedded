#include <avr/io.h>
// #include <util/twi.h>

void uart_init(void) {
  unsigned char ubrr;

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

void uart_tx(char c) {
  /* Wait for empty transmit buffer */
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  /*copy data to be sent*/
  UDR0 = c;
}

void uart_printstr(const char *str) {
  int i;

  i = 0;
  while (str[i]) {
    while (!(UCSR0A & (1 << UDRE0))) {
    }
    /*copy data to be sent*/
    UDR0 = str[i];
    i++;
  }
}

void ft_putnbr(int16_t nbr) {
  if (nbr < 0) {
    uart_tx('-');
    nbr = -nbr;
  }
  if (nbr < 10) {
    uart_tx(nbr + '0');
  } else {
    ft_putnbr(nbr / 10);
    ft_putnbr(nbr % 10);
  }
}
