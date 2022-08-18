
#include "HAL_ATMega4809.h"
#include "uart.h"
#include <util/delay.h>
#include <avr/interrupt.h>

// IO Definitions
#define __PIN_CTRL_REG__(port, pin) __PORT__(port).__PIN_CTRL__(pin)
#define __PORT__(port) PORT##port
#define __PIN_CTRL__(pin) PIN##pin##CTRL

#define _pin_control(port,pin) port ## PIN ## pin ##CTRL

extern volatile PwmInputCapture input_capture;

static void setup_input_capture() {
    // Configure interrupt on input capture pins
    __PIN_CTRL_REG__(PWM_IN_1_PORT, PWM_IN_1_PIN) |= PORT_ISC_BOTHEDGES_gc;
    __PIN_CTRL_REG__(PWM_IN_2_PORT, PWM_IN_2_PIN) |= PORT_ISC_BOTHEDGES_gc;
    __PIN_CTRL_REG__(PWM_IN_3_PORT, PWM_IN_3_PIN) |= PORT_ISC_BOTHEDGES_gc; 
    __PIN_CTRL_REG__(PWM_IN_4_PORT, PWM_IN_4_PIN) |= PORT_ISC_BOTHEDGES_gc;
    
    // Start input capture timers
    TCB0.CCMP = 0xFFFF; // Set TOP value to MAX
    TCB0.CTRLA = TCB_CLKSEL_CLKDIV2_gc | 0x01; // Enable & set to clkdiv2
    TCB1.CCMP = 0xFFFF; // Set TOP value to MAX
    TCB1.CTRLA = TCB_CLKSEL_CLKDIV2_gc | 0x01; // Enable & set to clkdiv2
    TCB2.CCMP = 0xFFFF; // Set TOP value to MAX
    TCB2.CTRLA = TCB_CLKSEL_CLKDIV2_gc | 0x01; // Enable & set to clkdiv2
    TCB3.CCMP = 0xFFFF; // Set TOP value to MAX
    TCB3.CTRLA = TCB_CLKSEL_CLKDIV2_gc | 0x01; // Enable & set to clkdiv2
    
    sei();
}

void platform_specific_setup() {
    // Set clock divider to 1
    CCP = CCP_IOREG_gc; //Configuration Change Protection
    CLKCTRL.MCLKCTRLB = 0; //Clock Div = 1
    
    uart0_init();
    
    // Setup input capture interrupts
    setup_input_capture();
}

/* 
 * Runs a test that blinks an LED on PIN A2 and writes acii text to UART0 (TX on PA0)
 */
void platform_specific_test() {
    PORTF.DIRSET = PIN3_bm; // Set as output
    
    uart0_send_string((char*)"Begin UART0 Test...\r\n\0");
    
    while (1) {
        PORTF.OUTTGL = PIN3_bm;
        uart0_send_string((char*)"---\r\n\0");
        uart0_send_string((char*)"1: \0");
        uart0_print_u16(input_capture.ch1_pulse_width_us);
        uart0_send_string((char*)"\r\n\0");
        
        uart0_send_string((char*)"2: \0");
        uart0_print_u16(input_capture.ch2_pulse_width_us);
        uart0_send_string((char*)"\r\n\0");
        
        uart0_send_string((char*)"3: \0");
        uart0_print_u16(input_capture.ch3_pulse_width_us);
        uart0_send_string((char*)"\r\n\0");
        
        uart0_send_string((char*)"4: \0");
        uart0_print_u16(input_capture.ch4_pulse_width_us);
        uart0_send_string((char*)"\r\n\0");
        _delay_ms(200);
    }
}

void platform_specific_write_string(char* string) {
    uart0_send_string(string);
}

ISR(PORTA_PORT_vect) {
    uint8_t pins = PORTA.IN;
    
    uint8_t interrupt_flags = PORTA.INTFLAGS; // store active flags
    PORTA.INTFLAGS = interrupt_flags; // Clear active flags
    
    if (interrupt_flags & PWM_IN_1_PIN_bm) {
        if (pins & PWM_IN_1_PIN_bm) {
            TCB0.CNT = 0;
        } else {
            input_capture.ch1_pulse_width_us = TCB0.CNT/10;
        }
    }
    if (interrupt_flags & PWM_IN_2_PIN_bm) {
        if (pins & PWM_IN_2_PIN_bm) {
            TCB1.CNT = 0;
        } else {
            input_capture.ch2_pulse_width_us = TCB1.CNT/10;
        }
    }
    if (interrupt_flags & PWM_IN_3_PIN_bm) {
        if (pins & PWM_IN_3_PIN_bm) {
            TCB2.CNT = 0;
        } else {
            input_capture.ch3_pulse_width_us = TCB2.CNT/10;
        }
    }
    if (interrupt_flags & PWM_IN_4_PIN_bm) {
        if (pins & PWM_IN_4_PIN_bm) {
            TCB3.CNT = 0;
        } else {
            input_capture.ch4_pulse_width_us = TCB3.CNT/10;
        }
    }
}

#undef __PIN_CTRL_REG__
#undef __PORT__
#undef __PIN_CTRL__