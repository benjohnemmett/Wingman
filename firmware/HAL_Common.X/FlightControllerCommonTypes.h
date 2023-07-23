/* 
 * File:   FlightControllerTypes.h
 * Author: Ben Emmett
 *
 * Created on August 15, 2022, 4:14 PM
 */

#ifndef FLIGHTCONTROLLERCOMMONTYPES_H
#define	FLIGHTCONTROLLERCOMMONTYPES_H

#ifdef	__cplusplus
extern "C" {
#endif

    struct PwmInputCapture {
        unsigned int ch1_pulse_width_us;
        unsigned int ch2_pulse_width_us;
        unsigned int ch3_pulse_width_us;
        unsigned int ch4_pulse_width_us;
    };
    typedef struct PwmInputCapture PwmInputCapture;

    struct PwmOutputData {
        unsigned int pulse_ticks[5];
        char current_channel;
    };
    typedef struct PwmOutputData PwmOutputData;
    

#ifdef	__cplusplus
}
#endif

#endif	/* FLIGHTCONTROLLERCOMMONTYPES_H */
