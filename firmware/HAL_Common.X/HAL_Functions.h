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

void HAL_setup();

void HAL_test();

void HAL_write_string(char* string);

void HAL_update_pwm_output(volatile PwmInputCapture *input, volatile PwmOutputData *output);

void HAL_print_test_data();


#ifdef	__cplusplus
}
#endif

#endif	/* HAL_FUNCTIONS_H */

