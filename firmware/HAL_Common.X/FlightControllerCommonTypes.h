/* 
 * File:   FlightControllerTypes.h
 * Author: Ben Emmett
 *
 * Created on August 15, 2022, 4:14 PM
 */
#include <stdint.h>

#ifndef FLIGHTCONTROLLERCOMMONTYPES_H
#define	FLIGHTCONTROLLERCOMMONTYPES_H

#ifdef	__cplusplus
extern "C" {
#endif

    struct PwmInputCapture {
        uint16_t ch1_pulse_width_us;
        uint16_t ch2_pulse_width_us;
        uint16_t ch3_pulse_width_us;
        uint16_t ch4_pulse_width_us;
    };
    typedef struct PwmInputCapture PwmInputCapture;

    struct PwmOutputData {
        uint16_t pulse_ticks[5];
        char current_channel;
    };
    typedef struct PwmOutputData PwmOutputData;
    

#ifdef	__cplusplus
}
#endif

#endif	/* FLIGHTCONTROLLERCOMMONTYPES_H */
