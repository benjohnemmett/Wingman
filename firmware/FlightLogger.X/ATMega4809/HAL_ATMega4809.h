/* 
 * File:   HAL_ATMega4809.h
 * Author: ben
 *
 * Created on July 9, 2023, 10:53 PM
 */

#ifndef HAL_ATMEGA4809_H
#define	HAL_ATMEGA4809_H

#include <avr/io.h>

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

    void HAL_hardware_setup();

    void HAL_print_string(char* string);
    
    void HAL_flash_status_lights();
    
#ifdef	__cplusplus
}
#endif

#endif	/* HAL_ATMEGA4809_H */

