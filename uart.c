#include <avr/io.h>
#include <stdio.h>

#ifndef F_CPU
#error "The CPU frequency MUST be set"
#endif


#ifndef BAUD
#define BAUD 9600
#endif
#include <util/setbaud.h>

#define MYUBRR (F_CPU/16/BAUD-1)

/* http://www.cs.mun.ca/~rod/Winter2007/4723/notes/serial/serial.html */

void uart_init(void) {
//    UBRRH = UBRRH_VALUE;
//    UBRRL = UBRRL_VALUE;

    /* Set baud rate */
    UBRRH = (unsigned char)(MYUBRR>>8);
    UBRRL = (unsigned char)MYUBRR;
    
    UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0); /* Set frame format: 8data, 2stop bit */
    UCSRB = _BV(RXEN) | _BV(TXEN);   /* Enable RX and TX */    
}

void uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
}

char uart_getchar(FILE *stream) {
    loop_until_bit_is_set(UCSRA, RXC);
    return UDR;
}

