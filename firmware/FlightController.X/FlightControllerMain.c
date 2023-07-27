/*
 * File:   FlightControllerMain.c
 * Author: Ben Emmett
 *
 * Created on July 15, 2022, 10:33 AM
 */
#include "../HAL_Common.X/FlightControllerCommonTypes.h"
#include "../HAL_ATMega4809.X/HAL_ATMega4809.h"
#include "PidController.h"

#define UPDATE_PERIOD_MS 10

PidControllerState rollPid;
PidControllerState pitchPid;

volatile PwmInputCapture pwm_input_capture;
volatile PwmOutputData pwm_output_data;

volatile unsigned char update_timer_expired = 0;

int main(void) {
    HAL_setup(&update_timer_expired);
    HAL_write_string((char*)"\r\nFC Main Starting Up\r\n\0");
    HAL_test();
    
    //Start Update timer
    while (1) {
        // Wait for TBD ms update period
//        while (!update_timer_expired) {
//        }
//        update_timer_expired = 0;
        
        // Get updated body state from IMU
        
        // Get latest RX control input
        
        float rollError = 0;
        float pitchError = 0;
        
        // Run control algorithm
        float rollCommand = RunPidController(&rollPid, rollError, UPDATE_PERIOD_MS);
        float pitchCommand = RunPidController(&pitchPid, pitchError, UPDATE_PERIOD_MS);
        
        // Update PWM controller
        HAL_update_pwm_output(&pwm_input_capture, &pwm_output_data);
        //HAL_print_test_data();
    }
}
