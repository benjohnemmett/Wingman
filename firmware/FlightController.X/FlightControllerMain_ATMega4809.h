/* 
 * File:   FlightControllerMain_ATMega4809.h
 * Author: Ben Emmett
 *
 * Created on August 9, 2022, 11:38 AM
 */

#ifndef FLIGHTCONTROLLERMAIN_ATMEGA4809_H
#define	FLIGHTCONTROLLERMAIN_ATMEGA4809_H

#include <avr/io.h>

#define __PIN_CTRL_REG__(port, pin) __PORT__(port).__PIN_CTRL__(pin)
#define __PORT__(port) PORT##port
#define __PIN_CTRL__(pin) PIN##pin##CTRL

#define PWM_IN_1_PORT A
#define PWM_IN_1_PIN 1
#define PWM_IN_2_PORT A
#define PWM_IN_2_PIN 2
#define PWM_IN_3_PORT A
#define PWM_IN_3_PIN 3
#define PWM_IN_4_PORT A
#define PWM_IN_4_PIN 3

#define _pin_control(port,pin) port ## PIN ## pin ##CTRL

#ifdef	__cplusplus
extern "C" {
#endif
    
    void setup_platform_specific_registers() {
        __PIN_CTRL_REG__(PWM_IN_1_PORT, PWM_IN_1_PIN) |= PORT_ISC_BOTHEDGES_gc;
        __PIN_CTRL_REG__(PWM_IN_2_PORT, PWM_IN_2_PIN) |= PORT_ISC_BOTHEDGES_gc;
        __PIN_CTRL_REG__(PWM_IN_3_PORT, PWM_IN_3_PIN) |= PORT_ISC_BOTHEDGES_gc;
        __PIN_CTRL_REG__(PWM_IN_4_PORT, PWM_IN_4_PIN) |= PORT_ISC_BOTHEDGES_gc;
    }

#ifdef	__cplusplus
}
#endif

#undef __PIN_CTRL_REG__
#undef __PORT__
#undef __PIN_CTRL__

#endif	/* FLIGHTCONTROLLERMAIN_ATMEGA4809_H */
