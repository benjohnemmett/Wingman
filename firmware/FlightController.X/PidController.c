#include "PidController.h"

float RunPidController(PidControllerState *state, float error, float delta_t_ms) {
    
    // Integrator
    state->integrator += state->Ki * error * delta_t_ms;
    
    // Clamp Integrator
    if (state->integrator > state->integrator_max) {
        state->integrator = state->integrator_max;
    } else if (state->integrator < state->integrator_min) {
        state->integrator = state->integrator_min;
    }
    
    // Differential
    float error_rate = 0;
    if (state->has_last_error) {
        error_rate = (error - state->last_error)/delta_t_ms;
    }
    
    float correction = (error * state->Kp) + (error_rate * state->Kd) + state->integrator;
    
    state->last_error = error;
    state->has_last_error = 1;
    
    return correction;
}

void ResetPidControllerState(PidControllerState *state) {
    state->Kp = 0;
    state->Ki = 0;
    state->Kd = 0;
    
    state->integrator_max = 100;
    state->integrator_min = -100;
    
    state->last_error = 0;
    state->has_last_error = 0;
    state->integrator = 0;
    PidControllerStatus status = PID_STATUS_OKAY;
}
