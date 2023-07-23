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

void platform_specific_setup();

void platform_specific_test();

void platform_specific_write_string(char* string);

void platform_specific_update_pwm_output(volatile PwmInputCapture *input, volatile PwmOutputData *output);

void platform_specific_print_test_data();


#ifdef	__cplusplus
}
#endif

#endif	/* HAL_FUNCTIONS_H */

