/* 
 * File:   FlightControllerTypes.h
 * Author: Ben Emmett
 *
 * Created on August 15, 2022, 4:14 PM
 */


#ifndef FLIGHTCONTROLLERCOMMONTYPES_H
#define	FLIGHTCONTROLLERCOMMONTYPES_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

    // Used to capture PWM using input interrupt and internal timer
    struct PwmInputCapture {
        uint16_t ch1_pulse_width_us;
        uint16_t ch2_pulse_width_us;
        uint16_t ch3_pulse_width_us;
        uint16_t ch4_pulse_width_us;
    };
    typedef struct PwmInputCapture PwmInputCapture;

    // Used for PWM output using internal timer
    struct PwmOutputData {
        uint16_t pulse_ticks[5];
        char current_channel;
    };
    typedef struct PwmOutputData PwmOutputData;
    
    // Used for generic output control. 
    // Value ranges are [-1, 1]
    struct ControlOutputData {
        float yaw_control;
        float pitch_control;
        float roll_control;
    };
    typedef struct ControlOutputData ControlOutputData;
    

#ifdef	__cplusplus
}
#endif

#endif	/* FLIGHTCONTROLLERCOMMONTYPES_H */
