
#include "HAL_ATMega4809.h"
#include "uart.h"
#include <util/delay.h>
#include <avr/interrupt.h>

// IO Definitions
#define __SET_OUTPUT_HIGH__(port, pin) __PIN_OUT_SET_REG__(port) |= BIT_VAL(pin)
#define __SET_OUTPUT_LOW__(port, pin) __PIN_OUT_CLR_REG__(port) |= BIT_VAL(pin)
#define __CONFIGURE_PIN_AS_OUTPUT__(port, pin) __PIN_DIR_REG__(port) |= BIT_VAL(pin)
#define __PIN_DIR_REG__(port) (__PORT_REG__(port).DIR)
#define __PIN_OUT_SET_REG__(port) (__PORT_REG__(port).OUTSET)
#define __PIN_OUT_CLR_REG__(port) (__PORT_REG__(port).OUTCLR)
#define __PORT_PIN_CTRL_REG__(port, pin) __PORT_REG__(port).__PIN_CTRL_REG__(pin)
#define __PORT_REG__(port) PORT##port
#define __PIN_CTRL_REG__(pin) PIN##pin##CTRL

#define MICROS_TO_TCA_TICKS_DIV8 2.5
#define MICROS_TO_TCA_TICKS_DIV16 1.25
#define SERVO_PWM_PERIOD_US 20000
#define SERVO_PWM_PERIOD_TICKS_DIV16 25000

#define INT_TRANSITION_CYCLES 8

extern volatile PwmInputCapture pwm_input_capture;
extern volatile PwmOutputData pwm_output_data;

/*************************************************************
 * Setup Functions 
 *************************************************************/
static void setup_output_pwm() {
    __CONFIGURE_PIN_AS_OUTPUT__(PWM_OUT_1_PORT, PWM_OUT_1_PIN);
    __CONFIGURE_PIN_AS_OUTPUT__(PWM_OUT_2_PORT, PWM_OUT_2_PIN);
    __CONFIGURE_PIN_AS_OUTPUT__(PWM_OUT_3_PORT, PWM_OUT_3_PIN);
    __CONFIGURE_PIN_AS_OUTPUT__(PWM_OUT_4_PORT, PWM_OUT_4_PIN);
    
    TCA0.SINGLE.PER = 0xFFFF; // Set to max, should never reach this though
    TCA0.SINGLE.CMP0 = 500 * MICROS_TO_TCA_TICKS_DIV16; // default value
    TCA0.SINGLE.INTCTRL |= TCA_SINGLE_CMP0_bm;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc | 0x01;
}

static void setup_input_capture() {
    // Configure interrupt on input capture pins
    __PORT_PIN_CTRL_REG__(PWM_IN_1_PORT, PWM_IN_1_PIN) |= PORT_ISC_BOTHEDGES_gc;
    __PORT_PIN_CTRL_REG__(PWM_IN_2_PORT, PWM_IN_2_PIN) |= PORT_ISC_BOTHEDGES_gc;
    __PORT_PIN_CTRL_REG__(PWM_IN_3_PORT, PWM_IN_3_PIN) |= PORT_ISC_BOTHEDGES_gc;
    __PORT_PIN_CTRL_REG__(PWM_IN_4_PORT, PWM_IN_4_PIN) |= PORT_ISC_BOTHEDGES_gc;
    
    // Start input capture timers
    TCB0.CCMP = 0xFFFF; // Set TOP value to MAX
    TCB0.CTRLA = TCB_CLKSEL_CLKDIV2_gc | 0x01; // Enable & set to clkdiv2
    TCB1.CCMP = 0xFFFF; // Set TOP value to MAX
    TCB1.CTRLA = TCB_CLKSEL_CLKDIV2_gc | 0x01; // Enable & set to clkdiv2
    TCB2.CCMP = 0xFFFF; // Set TOP value to MAX
    TCB2.CTRLA = TCB_CLKSEL_CLKDIV2_gc | 0x01; // Enable & set to clkdiv2
    TCB3.CCMP = 0xFFFF; // Set TOP value to MAX
    TCB3.CTRLA = TCB_CLKSEL_CLKDIV2_gc | 0x01; // Enable & set to clkdiv2
}

void platform_specific_setup() {
    // Set CPU clock divider to 1
    CCP = CCP_IOREG_gc; //Configuration Change Protection
    CLKCTRL.MCLKCTRLB = 0; //Clock Div = 1
    
    uart0_init();
    
    setup_input_capture();
    setup_output_pwm();
    
    sei();
}

/*************************************************************
 * Update Functions 
 *************************************************************/
void platform_specific_update_pwm_output(volatile PwmInputCapture *input, volatile PwmOutputData *output) {
    cli();
    output->pulse_ticks[0] = (unsigned int)((float)input->ch1_pulse_width_us * MICROS_TO_TCA_TICKS_DIV16) - INT_TRANSITION_CYCLES;
    output->pulse_ticks[1] = (unsigned int)((float)input->ch2_pulse_width_us * MICROS_TO_TCA_TICKS_DIV16) - INT_TRANSITION_CYCLES;
    output->pulse_ticks[2] = (unsigned int)((float)input->ch3_pulse_width_us * MICROS_TO_TCA_TICKS_DIV16) - INT_TRANSITION_CYCLES;
    output->pulse_ticks[3] = (unsigned int)((float)input->ch4_pulse_width_us * MICROS_TO_TCA_TICKS_DIV16) - INT_TRANSITION_CYCLES;
    unsigned int all_channels_sum_us = input->ch1_pulse_width_us + 
                                        input->ch2_pulse_width_us + 
                                        input->ch3_pulse_width_us + 
                                        input->ch4_pulse_width_us;
    if (all_channels_sum_us < SERVO_PWM_PERIOD_US) {
        output->pulse_ticks[4] = ((unsigned int)((float)(SERVO_PWM_PERIOD_US - all_channels_sum_us)) * MICROS_TO_TCA_TICKS_DIV16) - INT_TRANSITION_CYCLES;
    } else {
        output->pulse_ticks[4] = 0;
    }
    sei();
}

/*************************************************************
 * Test Functions 
 *************************************************************/
/* 
 * Write ASCII text to UART0 (TX on PA0)
 */
void print_pwm_input_capture() {
    uart0_send_string((char*)"---\r\n\0");
    uart0_send_string((char*)"1: \0");
    uart0_print_u16(pwm_input_capture.ch1_pulse_width_us);
    uart0_send_string((char*)"\r\n\0");

    uart0_send_string((char*)"2: \0");
    uart0_print_u16(pwm_input_capture.ch2_pulse_width_us);
    uart0_send_string((char*)"\r\n\0");

    uart0_send_string((char*)"3: \0");
    uart0_print_u16(pwm_input_capture.ch3_pulse_width_us);
    uart0_send_string((char*)"\r\n\0");

    uart0_send_string((char*)"4: \0");
    uart0_print_u16(pwm_input_capture.ch4_pulse_width_us);
    uart0_send_string((char*)"\r\n\0");
    _delay_ms(200);
}

/* 
 * Blink a light and print test data
 */
void platform_specific_test() {
    PORTE.DIRSET = PIN3_bm; // Set as output
    
    volatile PwmInputCapture test_input;
    test_input.ch1_pulse_width_us = 1000;
    test_input.ch2_pulse_width_us = 1300;
    test_input.ch3_pulse_width_us = 1600;
    test_input.ch4_pulse_width_us = 2000;
    
    platform_specific_update_pwm_output(&test_input, &pwm_output_data);
    pwm_output_data.current_channel = 4;
    
    uart0_send_string((char*)"Begin UART0 Test...\r\n\0");
    
    while (1) {
        PORTE.OUTTGL = PIN3_bm;
        print_pwm_input_capture();
    }
}

/* 
 * Write string to UART
 */
void platform_specific_write_string(char* string) {
    uart0_send_string(string);
}


/*************************************************************
 * ISRs 
 *************************************************************/
ISR(TCA0_CMP0_vect) {
    TCA0.SINGLE.INTFLAGS |= TCA_SINGLE_CMP0_bm; // clear interrupt flag
    
    uint8_t next_channel = pwm_output_data.current_channel + 1;
    
    if (pwm_output_data.current_channel == 0) {
        __SET_OUTPUT_LOW__(PWM_OUT_1_PORT, PWM_OUT_1_PIN);
        __SET_OUTPUT_HIGH__(PWM_OUT_2_PORT, PWM_OUT_2_PIN);
    } else if (pwm_output_data.current_channel == 1) {
        __SET_OUTPUT_LOW__(PWM_OUT_2_PORT, PWM_OUT_2_PIN);
        __SET_OUTPUT_HIGH__(PWM_OUT_3_PORT, PWM_OUT_3_PIN);
    } else if (pwm_output_data.current_channel == 2) {
        __SET_OUTPUT_LOW__(PWM_OUT_3_PORT, PWM_OUT_3_PIN);
        __SET_OUTPUT_HIGH__(PWM_OUT_4_PORT, PWM_OUT_4_PIN);
    } else if (pwm_output_data.current_channel == 3) {
        __SET_OUTPUT_LOW__(PWM_OUT_4_PORT, PWM_OUT_4_PIN);
    } else if (pwm_output_data.current_channel == 4) {
        __SET_OUTPUT_HIGH__(PWM_OUT_1_PORT, PWM_OUT_1_PIN);
        next_channel = 0;
    }
    
    TCA0.SINGLE.CNT = 0; // Reset the timer
    TCA0.SINGLE.CMP0 = pwm_output_data.pulse_ticks[next_channel];
    pwm_output_data.current_channel = next_channel;
}

ISR(PORTA_PORT_vect) {
    uint8_t pins = PORTA.IN;
    uint8_t interrupt_flags = PORTA.INTFLAGS; // store active flags
    PORTA.INTFLAGS = interrupt_flags; // Clear active flags
    
    if (interrupt_flags & PWM_IN_1_PIN_bm) {
        if (pins & PWM_IN_1_PIN_bm) {
            TCB0.CNT = 0;
        } else {
            pwm_input_capture.ch1_pulse_width_us = TCB0.CNT/10;
        }
    }
    if (interrupt_flags & PWM_IN_2_PIN_bm) {
        if (pins & PWM_IN_2_PIN_bm) {
            TCB1.CNT = 0;
        } else {
            pwm_input_capture.ch2_pulse_width_us = TCB1.CNT/10;
        }
    }
    if (interrupt_flags & PWM_IN_3_PIN_bm) {
        if (pins & PWM_IN_3_PIN_bm) {
            TCB2.CNT = 0;
        } else {
            pwm_input_capture.ch3_pulse_width_us = TCB2.CNT/10;
        }
    }
    if (interrupt_flags & PWM_IN_4_PIN_bm) {
        if (pins & PWM_IN_4_PIN_bm) {
            TCB3.CNT = 0;
        } else {
            pwm_input_capture.ch4_pulse_width_us = TCB3.CNT/10;
        }
    }
}

#undef __PIN_CTRL_REG__
#undef __PORT_REG__
#undef __PIN_CTRL_REG__