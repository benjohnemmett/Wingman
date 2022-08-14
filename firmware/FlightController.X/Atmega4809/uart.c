#include "uart.h"

//Helpful example: https://github.com/MicrochipTech/TB3216_Getting_Started_with_USART/blob/master/Send_Hello_World/main.c

// TODO BJE - make generic for any of the UARTs (0, 1, 2, or 3)
void uart0_init() {
    //1. Set the baud rate (USARTn.BAUD).
    USART0.BAUD = (uint16_t)USART0_BAUD_RATE(9600);

    //2. Set the frame format and mode of operation (USARTn.CTRLC).
    USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | 
                   USART_PMODE_DISABLED_gc | 
                   USART_SBMODE_1BIT_gc |
                   USART_CHSIZE_8BIT_gc;

    //3. Configure the TXD pin as an output.
    PORTA.DIRSET = PIN0_bm;
    PORTA.DIRCLR = PIN1_bm;

    //4. Enable the transmitter and the receiver (USARTn.CTRLB).
    USART0.CTRLB = USART_TXEN_bm | 
                   USART_RXEN_bm;
}

void uart0_send_char(char value) {
    while(!(USART0.STATUS & USART_DREIF_bm)) {}
    USART0.TXDATAL = value;
}

void uart0_send_string(char *string) {
    while (string[0]) {
        uart0_send_char(string[0]);
        string++;
    }
}