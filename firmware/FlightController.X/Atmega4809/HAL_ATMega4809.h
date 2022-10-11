/* 
 * File:   HAL_ATMega4809.h
 * Author: ben
 *
 * Created on August 13, 2022, 12:28 PM
 */

#ifndef HAL_ATMEGA4809_H
#define	HAL_ATMEGA4809_H

#include <avr/io.h>
#include "../FlightControllerTypes.h"

#define F_CPU 20000000

#define PWM_IN_1_PORT A
#define PWM_IN_1_PIN 5
#define PWM_IN_1_PIN_bm (0x01 << PWM_IN_1_PIN)
#define PWM_IN_2_PORT A
#define PWM_IN_2_PIN 7
#define PWM_IN_2_PIN_bm (0x01 << PWM_IN_2_PIN)
#define PWM_IN_3_PORT B
#define PWM_IN_3_PIN 1
#define PWM_IN_3_PIN_bm (0x01 << PWM_IN_3_PIN)
#define PWM_IN_4_PORT B
#define PWM_IN_4_PIN 3
#define PWM_IN_4_PIN_bm (0x01 << PWM_IN_4_PIN)

#define PWM_OUT_1_PORT F
#define PWM_OUT_1_PIN 2
#define PWM_OUT_2_PORT F
#define PWM_OUT_2_PIN 0
#define PWM_OUT_3_PORT E
#define PWM_OUT_3_PIN 2
#define PWM_OUT_4_PORT E
#define PWM_OUT_4_PIN 0

#define STAT_LED_1_PORT D
#define STAT_LED_1_PIN 0
#define STAT_LED_2_PORT D
#define STAT_LED_2_PIN 1
#define STAT_LED_3_PORT D
#define STAT_LED_3_PIN 2
#define STAT_LED_4_PORT D
#define STAT_LED_4_PIN 3
#define STAT1_OFF() PORTD.OUT |= (1 << STAT_LED_1_PIN);
#define STAT2_OFF() PORTD.OUT |= (1 << STAT_LED_2_PIN);
#define STAT3_OFF() PORTD.OUT |= (1 << STAT_LED_3_PIN);
#define STAT4_OFF() PORTD.OUT |= (1 << STAT_LED_4_PIN);
#define STAT1_ON() PORTD.OUT &= ~(1 << STAT_LED_1_PIN);
#define STAT2_ON() PORTD.OUT &= ~(1 << STAT_LED_2_PIN);
#define STAT3_ON() PORTD.OUT &= ~(1 << STAT_LED_3_PIN);
#define STAT4_ON() PORTD.OUT &= ~(1 << STAT_LED_4_PIN);

#define DIP1_PORT F
#define DIP1_PIN 1
#define DIP2_PORT F
#define DIP2_PIN 2

#define BIT_VAL(pin) (0x01 << pin)

#define UART_TX_PORT A
#define UART_TX_PIN 0
#define UART_RX_PORT A
#define UART_RX_PIN 1

struct HardwareConfiguration{
    uint8_t dip_1;
    uint8_t dip_2;
};
typedef struct HardwareConfiguration HardwareConfiguration;

#ifdef	__cplusplus
extern "C" {
#endif

void platform_specific_setup();

void platform_specific_test();

void platform_specific_write_string(char* string);

void platform_specific_update_pwm_output(volatile PwmInputCapture *input, volatile PwmOutputData *output);

void platform_specific_print_test_data();

#ifdef	__cplusplus
}
#endif

#endif	/* HAL_ATMEGA4809_H */

