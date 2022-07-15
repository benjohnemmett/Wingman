#include "PidController.h"

float RunPidController(PidControllerState *state, float error, float delta_t_ms) {
    (*state).error_sum += error * delta_t_ms;
    float error_rate = (error - (*state).last_error)/delta_t_ms;
    
    float correction = error*(*state).Kp + error_rate*(*state).Kd + (*state).error_sum*(*state).Ki;
    
    (*state).last_error = error;
    
    return correction;
}
