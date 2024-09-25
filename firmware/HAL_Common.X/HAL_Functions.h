/* 
 * File:   HAL_Functions.h
 * Author: ben
 *
 * Created on July 22, 2023, 11:04 PM
 */

#ifndef HAL_FUNCTIONS_H
#define	HAL_FUNCTIONS_H

#include <stdint.h>
#include "FlightControllerCommonTypes.h"

#ifdef	__cplusplus
extern "C" {
#endif

// Hardware specific setup, called once at startup
void HAL_setup(volatile unsigned char* update_timer_expired);

// Configures and enables hardware necessary for capturing PWM in
void HAL_setup_pwm_input_capture();

// Configures and enables hardware necessary for generating PWM output
void HAL_setup_pwm_output();

// Write PWM input to PWM output
void HAL_update_pwm_output_passthrough(volatile PwmInputCapture *input, volatile PwmOutputData *output);

// Update hardware for PWM output values
void HAL_update_pwm_output(ControlOutputData *output_control);

// Write string to default serial output
void HAL_write_string(char* string);

// Hardware specific sleep function
void HAL_sleep_ms(uint16_t sleep_ms);

// Hardware specific status light setter
void HAL_set_status_light(uint8_t index, uint8_t status);

// Hardware specific configuration getter. 2 LSBs map to dip switch values.
uint8_t HAL_get_hw_config_mode(); // Returns the mode defined by the DIP switches, 0, 1, 2 or 3.

// Open ended debug function 
void HAL_print_test_data();

// Open ended debug function
void HAL_test();


struct HardwareConfiguration{
    uint8_t dip_1;
    uint8_t dip_2;
};
typedef struct HardwareConfiguration HardwareConfiguration;

#ifdef	__cplusplus
}
#endif

#endif	/* HAL_FUNCTIONS_H */

