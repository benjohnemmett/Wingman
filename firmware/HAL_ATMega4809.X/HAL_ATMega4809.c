#include "HAL_ATMega4809.h"

#define USE_I2C_PCA9685

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
#define TCA_CLOCKSEL TCA_SINGLE_CLKSEL_DIV16_gc
#define MICROS_TO_TCA_TICKS MICROS_TO_TCA_TICKS_DIV16
#define SERVO_PWM_PERIOD_US 20000
#define SERVO_PWM_PERIOD_TICKS_DIV16 25000

#define INT_TRANSITION_CYCLES 8

#define ENABLE_TIMER_A() TCA0.SINGLE.CTRLA |= 0x01
#define DISABLE_TIMER_A() TCA0.SINGLE.CTRLA &= ~0x01

extern volatile PwmInputCapture pwm_input_capture; // TODO I think I'd rather this be passed in through function call instead of using extern
extern volatile PwmOutputData pwm_output_data;
volatile uint8_t *update_timer_expired_ptr;

HardwareConfiguration hw_config;

/*************************************************************
 * Setup Functions 
 *************************************************************/
static void setup_output_pwm() {
#ifdef USE_I2C_PCA9685
    SetupPca9685(PCA9685_PRESCALE_50HZ);
#else
    __CONFIGURE_PIN_AS_OUTPUT__(PWM_OUT_1_PORT, PWM_OUT_1_PIN);
    __CONFIGURE_PIN_AS_OUTPUT__(PWM_OUT_2_PORT, PWM_OUT_2_PIN);
    __CONFIGURE_PIN_AS_OUTPUT__(PWM_OUT_3_PORT, PWM_OUT_3_PIN);
    __CONFIGURE_PIN_AS_OUTPUT__(PWM_OUT_4_PORT, PWM_OUT_4_PIN);
    
    TCA0.SINGLE.PER = 0xFFFF; // Set to max, should never reach this though
    TCA0.SINGLE.CMP0 = 500 * MICROS_TO_TCA_TICKS; // default value
    TCA0.SINGLE.INTCTRL |= TCA_SINGLE_CMP0_bm;
    TCA0.SINGLE.CTRLA = TCA_CLOCKSEL | 0x01;
#endif
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

void setup_status_lights() {
    __CONFIGURE_PIN_AS_OUTPUT__(STAT_LED_1_PORT, STAT_LED_1_PIN);
    __CONFIGURE_PIN_AS_OUTPUT__(STAT_LED_2_PORT, STAT_LED_2_PIN);
    __CONFIGURE_PIN_AS_OUTPUT__(STAT_LED_3_PORT, STAT_LED_3_PIN);
    __CONFIGURE_PIN_AS_OUTPUT__(STAT_LED_4_PORT, STAT_LED_4_PIN);
    
    STAT1_OFF();
    STAT2_OFF();
    STAT3_OFF();
    STAT4_OFF();
    
    STAT1_ON();
    _delay_ms(500);
    STAT1_OFF();
    STAT2_ON();
    _delay_ms(500);
    STAT2_OFF();
    STAT3_ON();
    _delay_ms(500);
    STAT3_OFF();
    STAT4_ON();
    _delay_ms(500);
    STAT4_OFF();
    
    if (hw_config.dip_1) {
        uart0_send_string((char*)"DIP 1 ON\r\n\0");
        STAT1_ON();
        _delay_ms(500);
        STAT1_OFF();
    } else {
        uart0_send_string((char*)"DIP 1 OFF\r\n\0");
    }
    if (hw_config.dip_2) {
        uart0_send_string((char*)"DIP 2 ON\r\n\0");
        STAT2_ON();
        _delay_ms(500);
        STAT2_OFF();
    } else {
        uart0_send_string((char*)"DIP 2 OFF\r\n");
    }
}

void read_dip_switches() {
    // Set internal pullups
    PORTF.PIN1CTRL |= PORT_PULLUPEN_bm;
    PORTF.PIN2CTRL |= PORT_PULLUPEN_bm;
    
    // Low assertion
    hw_config.dip_1 = !(PORTF.IN & (1 << DIP1_PIN));
    hw_config.dip_2 = !(PORTF.IN & (1 << DIP2_PIN));
}

void HAL_setup(volatile uint8_t *new_update_timer_expired_ptr) {
    update_timer_expired_ptr = new_update_timer_expired_ptr;
    
    // Set CPU clock divider to 1
    CCP = CCP_IOREG_gc; //Configuration Change Protection
    CLKCTRL.MCLKCTRLB = 0; //Clock Div = 1
    uart0_init(9600);
    
    read_dip_switches();
    setup_status_lights();
    
    // Initialize RTC for main loop clock
    // Datasheet 22.4
    
    // Check RTC.STATUS and RTC.PITSTATUS Synchronization busy bits before using
    while (RTC.STATUS != 0) {  } // make sure the RTC is free
    while (RTC.PITSTATUS != 0) {  } // make sure the RTC PIT is free
    
    RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;
    RTC.PER = 328; // ~10ms
    RTC.INTCTRL = 0x01; // Enable OVF Interrupt
    RTC.CTRLA = RTC_PRESCALER_DIV1_gc | 0x01; // set prescaler and enable RTC
}

void HAL_setup_pwm_input_capture() {
    setup_input_capture();
    sei();
}

void HAL_setup_pwm_output() {
    setup_output_pwm();
    sei();
}

/*************************************************************
 * Update Functions 
 *************************************************************/

/*
 * Updates servo control using ControlOutputData
 */
void HAL_update_pwm_output(ControlOutputData *output_control) {
    SetServo(YAW_CHANNEL, output_control->yaw_control);
    SetServo(PITCH_CHANNEL, output_control->pitch_control);
    SetServo(ROLL_CHANNEL, output_control->roll_control);
}

/*
 * Update output data to match input capture
 */
void HAL_update_pwm_output_passthrough(volatile PwmInputCapture *input, volatile PwmOutputData *output) {
    cli();
    uint16_t in_1 = input->ch1_pulse_width_us;
    uint16_t in_2 = input->ch2_pulse_width_us;
    uint16_t in_3 = input->ch3_pulse_width_us;
    uint16_t in_4 = input->ch4_pulse_width_us;
    sei();
    
    // Clamp inputs
    if (in_1 > 2000) {
        in_1 = 2000;
    } else if (in_1 < 1000) {
        in_1 = 1000;
    }
    if (in_2 > 2000) {
        in_2 = 2000;
    } else if (in_2 < 1000) {
        in_2 = 1000;
    }
    if (in_3 > 2000) {
        in_3 = 2000;
    } else if (in_3 < 1000) {
        in_3 = 1000;
    }
    if (in_4 > 2000) {
        in_4 = 2000;
    } else if (in_4 < 1000) {
        in_4 = 1000;
    }
    
#if TCA_CLOCKSEL == TCA_SINGLE_CLKSEL_DIV16_gc
    // out = in * 1.25
    uint16_t out_1 = in_1 + (in_1 >> 2) - INT_TRANSITION_CYCLES;
    uint16_t out_2 = in_2 + (in_2 >> 2) - INT_TRANSITION_CYCLES;
    uint16_t out_3 = in_3 + (in_3 >> 2) - INT_TRANSITION_CYCLES;
    uint16_t out_4 = in_4 + (in_4 >> 2) - INT_TRANSITION_CYCLES;
#elif TCA_CLOCKSEL == TCA_SINGLE_CLKSEL_DIV8_gc
    // out = in * 2.5
    uint16_t out_1 = 2*in_1 + (in_1 >> 1) - INT_TRANSITION_CYCLES;
    uint16_t out_2 = 2*in_2 + (in_2 >> 1) - INT_TRANSITION_CYCLES;
    uint16_t out_3 = 2*in_3 + (in_3 >> 1) - INT_TRANSITION_CYCLES;
    uint16_t out_4 = 2*in_4 + (in_4 >> 1) - INT_TRANSITION_CYCLES;
#else
    uint16_t out_1 = (uint16_t)((float)in_1 * MICROS_TO_TCA_TICKS) - INT_TRANSITION_CYCLES;
    uint16_t out_2 = (uint16_t)((float)in_2 * MICROS_TO_TCA_TICKS) - INT_TRANSITION_CYCLES;
    uint16_t out_3 = (uint16_t)((float)in_3 * MICROS_TO_TCA_TICKS) - INT_TRANSITION_CYCLES;
    uint16_t out_4 = (uint16_t)((float)in_4 * MICROS_TO_TCA_TICKS) - INT_TRANSITION_CYCLES;
#endif
    
    uint32_t all_channels_sum_us = in_1 + 
                                        in_2 + 
                                        in_3 + 
                                        in_4;
    uint16_t out_extra;
    if (all_channels_sum_us < SERVO_PWM_PERIOD_US) {
        out_extra = ((unsigned int)((float)(SERVO_PWM_PERIOD_US - all_channels_sum_us)) * MICROS_TO_TCA_TICKS) - INT_TRANSITION_CYCLES;
    } else {
        out_extra = 0;
    }
    
    cli();
    output->pulse_ticks[0] = out_1;
    output->pulse_ticks[1] = out_2;
    output->pulse_ticks[2] = out_3;
    output->pulse_ticks[3] = out_4;
    output->pulse_ticks[4] = out_extra;
    sei();
    
    _delay_ms(50);
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
void HAL_test() {
    uart0_send_string((char*)"Begin HAL_test ...\r\n\0");
    
    //Set PWM 1-4 as outputs

    PORTA.DIRSET |= PIN5_bm; // Set pin as output
    PORTA.DIRSET |= PIN6_bm;
    PORTA.DIRSET |= PIN7_bm;
    PORTB.DIRSET |= PIN0_bm;
    PORTB.DIRSET |= PIN1_bm;
    PORTB.DIRSET |= PIN2_bm;
    
    PORTA.OUTCLR |= PIN5_bm; // Set output low
    PORTA.OUTCLR |= PIN6_bm;
    PORTA.OUTCLR |= PIN7_bm;
    PORTB.OUTCLR |= PIN0_bm;
    PORTB.OUTCLR |= PIN1_bm;
    PORTB.OUTCLR |= PIN2_bm;
   
    
    while (1) {
        uart0_send_string((char*)"Start Loop\r\n\0");
        PORTA.OUTSET |= PIN5_bm;
        _delay_ms(200);
        PORTA.OUTCLR |= PIN5_bm;
        _delay_ms(200);
        
        PORTA.OUTSET |= PIN6_bm;
        _delay_ms(200);
        PORTA.OUTCLR |= PIN6_bm;
        _delay_ms(200);
        
        PORTA.OUTSET |= PIN7_bm;
        _delay_ms(200);
        PORTA.OUTCLR |= PIN7_bm;
        _delay_ms(200);
        
        PORTB.OUTSET |= PIN0_bm;
        _delay_ms(200);
        PORTB.OUTCLR |= PIN0_bm;
        _delay_ms(200);
        
        PORTB.OUTSET |= PIN1_bm;
        _delay_ms(200);
        PORTB.OUTCLR |= PIN1_bm;
        _delay_ms(200);
        
        PORTB.OUTSET |= PIN2_bm;
        _delay_ms(200);
        PORTB.OUTCLR |= PIN2_bm;
        _delay_ms(200);
    }
}

void HAL_print_test_data() {
    print_pwm_input_capture();
}

/* 
 * Write string to UART
 */
void HAL_write_string(char* string) {
    uart0_send_string(string);
}

void HAL_sleep_ms(uint16_t sleep_ms) {
    while (sleep_ms > 1000) {
        _delay_ms(1000);
        sleep_ms -= 1000;
    }
    while (sleep_ms > 10) {
        _delay_ms(10);
        sleep_ms -= 10;
    }
    while (sleep_ms > 10) {
        _delay_ms(10);
        sleep_ms -= 10;
    }
    while (sleep_ms > 1) {
        _delay_ms(1);
        sleep_ms -= 1;
    }
}


void HAL_set_status_light(uint8_t index, uint8_t status)
{
    switch (index)
    {
        case 0:
            status ? STAT1_ON() : STAT1_OFF();
            break;
        case 1:
            status ? STAT2_ON() : STAT2_OFF();
            break;
        case 2:
            status ? STAT3_ON() : STAT3_OFF();
            break;
        case 3:
            status ? STAT4_ON() : STAT4_OFF();
            break;
        default:
            break;
    }
}


/*************************************************************
 * ISRs 
 *************************************************************/

// Drive manual PWM out for 4 separate channels 
//   Each PWM channel is updated sequentially starting at channel 1, then when
//   Channel 1 goes low, channel 2 goes high, and so on. After channel 4 goes low
//   all channels are held low to complete a 20m (50Hz) period from when channel 1 went high.
//   * I'm looking at this code a good amount of time after having written it, 
//   I assume that pwm_output_data.current_channel[5] must be set to (20ms - sum(pwm_output_data.current_channel[:4]))
//   in order to ensure a 20ms PWM period on each channel. 
//   ** Also it seems as though a sharp change in the pulse width of any channel 
//   would cause jitter in the 20ms period of the other channels. 
ISR(TCA0_CMP0_vect) {
    DISABLE_TIMER_A();
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
    pwm_output_data.current_channel = next_channel;
    
    TCA0.SINGLE.CNT = 0; // Reset the timer
    TCA0.SINGLE.CMP0 = pwm_output_data.pulse_ticks[next_channel];
    ENABLE_TIMER_A();
}


ISR(PORTA_PORT_vect) {
    uint8_t sreg = SREG;
    uint8_t pins = PORTA.IN;
    uint8_t int_flags = PORTA.INTFLAGS;
    PORTA.INTFLAGS |= int_flags;
    
    if (int_flags & PIN5_bm) {
        if (pins & PIN5_bm) {
            TCB0.CNT = 0;
            STAT1_ON();
        } else {
            pwm_input_capture.ch1_pulse_width_us = TCB0.CNT/10;
            STAT1_OFF();
        }
    }
    if (int_flags & PIN6_bm) {
        if (pins & PIN6_bm) {
            STAT2_ON();
            TCB1.CNT = 0;
        } else {
            pwm_input_capture.ch2_pulse_width_us = TCB1.CNT/10;
            STAT2_OFF();
        }
    }
    if (int_flags & PIN7_bm) {
        if (pins & PIN7_bm) {
            TCB2.CNT = 0;
            STAT3_ON();
        } else {
            pwm_input_capture.ch3_pulse_width_us = TCB2.CNT/10;
            STAT3_OFF();
        }
    }

    SREG = sreg;
}

ISR(PORTB_PORT_vect) {
    uint8_t sreg = SREG;
    uint8_t pins = PORTB.IN;
    uint8_t interrupt_flags = PORTB.INTFLAGS; // store active flags
    PORTB.INTFLAGS |= interrupt_flags;
    // PWM 4 IN
    if (interrupt_flags & PIN0_bm) {
        if (pins & PIN0_bm) {
            TCB3.CNT = 0;
            STAT4_ON();
        } else {
            pwm_input_capture.ch4_pulse_width_us = TCB3.CNT/10;
            STAT4_OFF();
        }
    }

    SREG = sreg;
}

ISR(RTC_CNT_vect) {
    uint8_t sreg = SREG;
    *update_timer_expired_ptr = 1;
    SREG = sreg;
}

#undef __PIN_CTRL_REG__
#undef __PORT_REG__
#undef __PIN_CTRL_REG__