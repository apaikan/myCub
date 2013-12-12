/* Based on Atmel Application Note AVR 306 */
#include <avr/io.h>

#ifndef UART_RX_BUFFER_SIZE
#define UART_RX_BUFFER_SIZE 64
#endif

#ifndef UART_TX_BUFFER_SIZE
#define UART_TX_BUFFER_SIZE 64
#endif

#ifndef BAUD
#define BAUD 9600
#endif
#include <util/setbaud.h>

struct tx_ring {
    int buffer[UART_TX_BUFFER_SIZE];
    int start;
    int end;
};

struct rx_ring {
    int buffer[UART_RX_BUFFER_SIZE];
    int start;
    int end;
};

static struct tx_ring tx_buffer;
static struct rx_ring rx_buffer;

#include <avr/interrupt.h>
#include <stdio.h>


/* http://www.cs.mun.ca/~rod/Winter2007/4723/notes/serial/serial.html */

extern int uart_output;
extern int uart_input;

void uart_init(void) {
    
    tx_buffer.start = 0;
    tx_buffer.end   = 0;

    rx_buffer.start = 0;
    rx_buffer.end   = 0;
    
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */  
    UCSR0B |=  (1 << RXCIE0) | (1 << RXEN0);
    stdout = &uart_output;
    stdin  = &uart_input;
    //sei();  
}

int uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    
    int write_pointer = (tx_buffer.end + 1) % UART_TX_BUFFER_SIZE;
    
    if (write_pointer != tx_buffer.start){
        tx_buffer.buffer[tx_buffer.end] = c;
        tx_buffer.end = write_pointer;
        
        /* Data available. Enable the transmit interrupt for serial port 0. */
        UCSR0B |= _BV(UDRIE0);  
    }
    
    return 0;
}


/*
int uart_getchar(FILE *stream) {
    int read_pointer = (rx_buffer.start + 1) % UART_RX_BUFFER_SIZE;
    
    rx_buffer.start = read_pointer;
    return rx_buffer.buffer[read_pointer];
}
*/


int uart_getchar(FILE *stream) {
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}



ISR(USART_RX_vect) 
{ 
    int write_pointer = (rx_buffer.end + 1) % UART_RX_BUFFER_SIZE;

    // Add next byte to ringbuffer if it has space available. 
    if (write_pointer != rx_buffer.start){
        rx_buffer.buffer[rx_buffer.end] = UDR0;
        rx_buffer.end = write_pointer;
    } 
    char c = UDR0;
}


ISR(USART_UDRE_vect){
    int read_pointer = (tx_buffer.start + 1) % UART_TX_BUFFER_SIZE;
       
    /* Transmit next byte if data available in ringbuffer. */
    if (read_pointer != tx_buffer.end) {
        UDR0 = tx_buffer.buffer[read_pointer];
        tx_buffer.start = read_pointer;
    } else {
        /* Nothing to send. Disable the transmit interrupt for serial port 0. */
        UCSR0B &= ~_BV(UDRIE0);
    }
}

