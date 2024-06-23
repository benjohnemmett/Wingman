/* 
 * File:   HAL_ATMega4809.h
 * Author: ben
 *
 * Created on August 13, 2022, 12:28 PM
 */

#ifndef HAL_ATMEGA4809_H
#define	HAL_ATMEGA4809_H

#define F_CPU 20000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../Libraries/mcu-device-util/uart/uart.h"
#include "../HAL_Common.X/HAL_Functions.h"
#include "../HAL_Common.X/FlightControllerCommonTypes.h"


#define PWM_IN_1_PORT A
#define PWM_IN_1_PIN 5
#define PWM_IN_1_PIN_bm (0x01 << PWM_IN_1_PIN)
#define PWM_IN_2_PORT A
#define PWM_IN_2_PIN 6
#define PWM_IN_2_PIN_bm (0x01 << PWM_IN_2_PIN)
#define PWM_IN_3_PORT A
#define PWM_IN_3_PIN 7
#define PWM_IN_3_PIN_bm (0x01 << PWM_IN_3_PIN)
#define PWM_IN_4_PORT B
#define PWM_IN_4_PIN 0
#define PWM_IN_4_PIN_bm (0x01 << PWM_IN_4_PIN)
#define PWM_IN_5_PORT B
#define PWM_IN_5_PIN 1
#define PWM_IN_5_PIN_bm (0x01 << PWM_IN_4_PIN)
#define PWM_IN_6_PORT B
#define PWM_IN_7_PIN 2
#define PWM_IN_8_PIN_bm (0x01 << PWM_IN_4_PIN)

#define PWM_OUT_1_PORT C
#define PWM_OUT_1_PIN 4
#define PWM_OUT_2_PORT C
#define PWM_OUT_2_PIN 5
#define PWM_OUT_3_PORT C
#define PWM_OUT_3_PIN 6
#define PWM_OUT_4_PORT C
#define PWM_OUT_4_PIN 7

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


#ifdef	__cplusplus
}
#endif

#endif	/* HAL_ATMEGA4809_H */

