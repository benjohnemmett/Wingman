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

/*
 *  Write the ASCII value of a decimal number. 
 *  Input must be in the range [0 9]
*/
char DecToAscii(uint8_t dec) {
    return (char)(dec + 0x30);
}

/*
 * Write the ASCII representation of the 16 bit unsigned value.
 */
void uart0_print_u16(uint16_t value) {
    uint8_t printZeros = 0;

    if (value > 9999) {
        uint8_t tenthousands = value / 10000;
        value -= 10000*tenthousands;
        uart0_send_char(DecToAscii(tenthousands));
        printZeros = 1;
    }

    if (value > 999) {
        uint8_t thousands = value / 1000;
        value -= 1000*thousands;
        uart0_send_char(DecToAscii(thousands));
        printZeros = 1;
    } else if (printZeros) {
        uart0_send_char(DecToAscii(0));
    }

    if (value > 99) {
        uint8_t hundreds = value / 100;
        value -= 100*hundreds;
        uart0_send_char(DecToAscii(hundreds));
        printZeros = 1;
    } else if (printZeros) {
        uart0_send_char(DecToAscii(0));
    }

    if (value > 9) {
        uint8_t tens = value / 10;
        value -= 10*tens;
        uart0_send_char(DecToAscii(tens));
    } else if (printZeros) {
        uart0_send_char(DecToAscii(0));
    }
    uart0_send_char(DecToAscii(value));
}
