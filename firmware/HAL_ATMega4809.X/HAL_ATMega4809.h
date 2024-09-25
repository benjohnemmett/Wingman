/* 
 * File:   HAL_ATMega4809.h
 * Author: ben
 *
 * Created on August 13, 2022, 12:28 PM
 */

#ifndef HAL_ATMEGA4809_H
#define	HAL_ATMEGA4809_H

#define F_CPU 20000000

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../Libraries/mcu-device-util/uart/uart.h"
#include "../HAL_Common.X/HAL_Functions.h"
#include "../HAL_Common.X/FlightControllerCommonTypes.h"
#include "../Libraries/mcu-device-util/PCA9685.X/PCA9685.h"

/* Peripheral Allocation
  TCA0  - PWM Servo CH 1-4 Output
  TCB0  - PWM CH 1 input
  TCB1  - PWM CH 2 input
  TCB2  - PWM CH 3 input
  TCB3  - PWM CH 4 input
  RTC   - 
  UART0 - 
  UART1 - 
  UART2 - 
  UART3 - 
  I2C   - MPU-6050/BMP-390/PCA9685
  SPI   - 
  ADC   - 
  GPIO D0 - STAT LED 1
  GPIO D1 - STAT LED 2
  GPIO D2 - STAT LED 3
  GPIO D3 - STAT LED 4
  GPIO F1 - DIP 1
  GPIO F2 - DIP 2
 * 
 * */

#define ROLL_CHANNEL 0
#define PITCH_CHANNEL 1
#define THROTTLE_CHANNEL 2
#define YAW_CHANNEL 3

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
#define STAT1_OFF() (PORTD.OUT |= (1 << STAT_LED_1_PIN))
#define STAT2_OFF() (PORTD.OUT |= (1 << STAT_LED_2_PIN))
#define STAT3_OFF() (PORTD.OUT |= (1 << STAT_LED_3_PIN))
#define STAT4_OFF() (PORTD.OUT |= (1 << STAT_LED_4_PIN))
#define STAT1_ON() PORTD.OUT &= ~(1 << STAT_LED_1_PIN)
#define STAT2_ON() PORTD.OUT &= ~(1 << STAT_LED_2_PIN)
#define STAT3_ON() PORTD.OUT &= ~(1 << STAT_LED_3_PIN)
#define STAT4_ON() PORTD.OUT &= ~(1 << STAT_LED_4_PIN)

#define DIP1_PORT F
#define DIP1_PIN 1
#define DIP2_PORT F
#define DIP2_PIN 2

#define BIT_VAL(pin) (0x01 << pin)

#define UART_TX_PORT A
#define UART_TX_PIN 0
#define UART_RX_PORT A
#define UART_RX_PIN 1

#ifdef	__cplusplus
extern "C" {
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* HAL_ATMEGA4809_H */

