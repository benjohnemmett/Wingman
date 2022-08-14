
#include "HAL_ATMega4809.h"
#include "uart.h"
#include <util/delay.h>

// IO Definitions
#define __PIN_CTRL_REG__(port, pin) __PORT__(port).__PIN_CTRL__(pin)
#define __PORT__(port) PORT##port
#define __PIN_CTRL__(pin) PIN##pin##CTRL

#define _pin_control(port,pin) port ## PIN ## pin ##CTRL


void platform_specific_setup() {
    // Set clock divider to 1
    CCP = CCP_IOREG_gc; //Configuration Change Protection
    CLKCTRL.MCLKCTRLB = 0; //Clock Div = 1
    
    uart0_init();

    // Setup input capture interrupts
//    __PIN_CTRL_REG__(PWM_IN_1_PORT, PWM_IN_1_PIN) |= PORT_ISC_BOTHEDGES_gc;
//    __PIN_CTRL_REG__(PWM_IN_2_PORT, PWM_IN_2_PIN) |= PORT_ISC_BOTHEDGES_gc;
//    __PIN_CTRL_REG__(PWM_IN_3_PORT, PWM_IN_3_PIN) |= PORT_ISC_BOTHEDGES_gc;
//    __PIN_CTRL_REG__(PWM_IN_4_PORT, PWM_IN_4_PIN) |= PORT_ISC_BOTHEDGES_gc;
}

/* 
 * Runs a test that blinks an LED on PIN A2 and writes acii text to UART0 (TX on PA0)
 */
void platform_specific_test() {
    PORTA.DIRSET = PIN2_bm; // Set PA0 as output
    
    uart0_send_string((char*)"Begin UART0 Test...\r\n\0");
    
    char offset = 0;
    while (1) {
        PORTA.OUTTGL = PIN2_bm;
        uart0_send_char('a' + offset++);
        if (offset >= 26) {
            uart0_send_string((char*)"\r\n\0");
            offset = 0;
        }
        _delay_ms(250);
    }
}

void platform_specific_write_string(char* string) {
    uart0_send_string(string);
}

#undef __PIN_CTRL_REG__
#undef __PORT__
#undef __PIN_CTRL__