/* 
 * File:   HAL_ATMega4809.h
 * Author: ben
 *
 * Created on August 13, 2022, 12:28 PM
 */

#ifndef HAL_ATMEGA4809_H
#define	HAL_ATMEGA4809_H

#include <avr/io.h>

#define F_CPU 20000000

#define PWM_IN_1_PORT A
#define PWM_IN_1_PIN 2
#define PWM_IN_1_PIN_bm (0x01 << PWM_IN_1_PIN)
#define PWM_IN_2_PORT A
#define PWM_IN_2_PIN 3
#define PWM_IN_2_PIN_bm (0x01 << PWM_IN_2_PIN)
#define PWM_IN_3_PORT A
#define PWM_IN_3_PIN 4
#define PWM_IN_3_PIN_bm (0x01 << PWM_IN_3_PIN)
#define PWM_IN_4_PORT A
#define PWM_IN_4_PIN 5
#define PWM_IN_4_PIN_bm (0x01 << PWM_IN_4_PIN)

#define UART_TX_PORT A
#define UART_TX_PIN 0
#define UART_RX_PORT A
#define UART_RX_PIN 1

#ifdef	__cplusplus
extern "C" {
#endif

void platform_specific_setup();

void platform_specific_test();

void platform_specific_write_string(char* string);

#ifdef	__cplusplus
}
#endif

#endif	/* HAL_ATMEGA4809_H */

