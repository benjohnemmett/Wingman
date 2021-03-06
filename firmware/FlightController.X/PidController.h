/* 
 * File:   PidController.h
 * Author: benjo
 *
 * Created on July 15, 2022, 10:42 AM
 */

#ifndef PIDCONTROLLER_H
#define	PIDCONTROLLER_H

#ifdef	__cplusplus
extern "C" {
#endif

    enum PidControllerStatus {
        PID_STATUS_OKAY,
        PID_STATUS_ERROR
    };
    typedef enum PidControllerStatus PidControllerStatus;
    
    struct PidControllerState{
        float Kp;
        float Ki;
        float Kd;
        
        float last_error;
        bool has_last_error = 0;
        float error_sum;
        PidControllerStatus status;
    };
    typedef struct PidControllerState PidControllerState;
    
    float RunPidController(PidControllerState *state, float error, float delta_t_ms);
    
#ifdef	__cplusplus
}
#endif

#endif	/* PIDCONTROLLER_H */

