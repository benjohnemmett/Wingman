/* 
 * File:   HAL_Functions.h
 * Author: ben
 *
 * Created on July 22, 2023, 11:04 PM
 */

#ifndef HAL_FUNCTIONS_H
#define	HAL_FUNCTIONS_H

#include "FlightControllerCommonTypes.h"

#ifdef	__cplusplus
extern "C" {
#endif

void HAL_setup(volatile unsigned char* update_timer_expired);

void HAL_setup_pwm_input_capture();

void HAL_setup_pwm_output();

void HAL_test();

void HAL_write_string(char* string);

void HAL_update_pwm_output(volatile PwmInputCapture *input, volatile PwmOutputData *output);

void HAL_print_test_data();

void HAL_sleep_ms(uint16_t sleep_ms);

#ifdef	__cplusplus
}
#endif

#endif	/* HAL_FUNCTIONS_H */

