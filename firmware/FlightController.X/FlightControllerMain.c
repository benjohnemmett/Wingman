/*
 * File:   FlightControllerMain.c
 * Author: benjo
 *
 * Created on July 15, 2022, 10:33 AM
 */

#include <avr/io.h>
#include "PidController.h"

#define UPDATE_PERIOD_MS 10

PidControllerState rollPid;
PidControllerState pitchPid;

volatile uint8_t update_timer_expired = 0;

int main(void) {
    
    // Initialize I2C for IMU
    // Initialize I2C for PWM controller
    
    //Start Update timer
    
    while (1) {
        // Wait for TBD ms update period
        while (!update_timer_expired) {
        }
        update_timer_expired = 0;
        
        // Get updated body state from IMU
        
        // Get latest RX control input
        
        float rollError = 0;
        float pitchError = 0;
        
        // Run control algorithm
        float rollCommand = RunPidController(&rollPid, rollError, UPDATE_PERIOD_MS);
        float pitchCommand = RunPidController(&pitchPid, pitchError, UPDATE_PERIOD_MS);
        
        // Update PWM controller
        
    }
}
