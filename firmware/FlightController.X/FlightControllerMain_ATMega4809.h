/* 
 * File:   FlightControllerMain_ATMega4809.h
 * Author: Ben Emmett
 *
 * Created on August 9, 2022, 11:38 AM
 */

#ifndef FLIGHTCONTROLLERMAIN_ATMEGA4809_H
#define	FLIGHTCONTROLLERMAIN_ATMEGA4809_H

#define F_CPU 20000000

#include <avr/io.h>
#include <util/delay.h>

// ATmega4809 Configuration Bit Settings

// 'C' source line config statements

#include <avr/io.h>

FUSES = {
	.WDTCFG = 0x00, // WDTCFG {PERIOD=OFF, WINDOW=OFF}
	.BODCFG = 0x00, // BODCFG {SLEEP=DIS, ACTIVE=DIS, SAMPFREQ=1KHZ, LVL=BODLEVEL0}
	.OSCCFG = 0x02, // OSCCFG {FREQSEL=20MHZ, OSCLOCK=CLEAR}
	.SYSCFG0 = 0xE4, // SYSCFG0 {EESAVE=CLEAR, RSTPINCFG=GPIO, CRCSRC=NOCRC}
	.SYSCFG1 = 0x07, // SYSCFG1 {SUT=64MS}
	.APPEND = 0x00, // APPEND {APPEND=User range:  0x0 - 0xFF}
	.BOOTEND = 0x00, // BOOTEND {BOOTEND=User range:  0x0 - 0xFF}
};

LOCKBITS = 0xC5; // {LB=NOLOCK}

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
        CCP = CCP_IOREG_gc; //Configuration Change Protection
        CLKCTRL.MCLKCTRLB = 0; //Clock Div = 1
        
        __PIN_CTRL_REG__(PWM_IN_1_PORT, PWM_IN_1_PIN) |= PORT_ISC_BOTHEDGES_gc;
        __PIN_CTRL_REG__(PWM_IN_2_PORT, PWM_IN_2_PIN) |= PORT_ISC_BOTHEDGES_gc;
        __PIN_CTRL_REG__(PWM_IN_3_PORT, PWM_IN_3_PIN) |= PORT_ISC_BOTHEDGES_gc;
        __PIN_CTRL_REG__(PWM_IN_4_PORT, PWM_IN_4_PIN) |= PORT_ISC_BOTHEDGES_gc;
    }
    
    void blink_test() {
        PORTA.DIRSET = PIN0_bm; // Set PA0 as outout
        while (1) {
            PORTA.OUTTGL = PIN0_bm;
            _delay_ms(250);
        }
    }

#ifdef	__cplusplus
}
#endif

#undef __PIN_CTRL_REG__
#undef __PORT__
#undef __PIN_CTRL__

#endif	/* FLIGHTCONTROLLERMAIN_ATMEGA4809_H */

