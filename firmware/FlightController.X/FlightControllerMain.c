/*
 * File:   FlightControllerMain.c
 * Author: Ben Emmett
 *
 * Created on July 15, 2022, 10:33 AM
 */
#include "../HAL_Common.X/FlightControllerCommonTypes.h"
#include "../HAL_Common.X/HAL_Functions.h"
#include "../HAL_ATMega4809.X/HAL_ATMega4809.h"
#include "../Libraries/mcu-device-util/MPU6050.X/mpu_6050.h"
#include "../Libraries/mcu-device-util/PCA9685.X/PCA9685.h"
#include "../Libraries/mcu-device-util/i2c/i2c.h"
#include "PidController.h"
#include <math.h>

#define F_CPU 20000000
#define BMP390_ADDRESS 0b01110110
#define UPDATE_PERIOD_MS 10

PidControllerState rollPid;
PidControllerState pitchPid;

uint16_t update_number = 0;

GyroscopeData gyroscope_data;
AccelerationData acceleration_data;
volatile PwmInputCapture pwm_input_capture;
volatile PwmOutputData pwm_output_data;
volatile ControlOutputData control_output_data;

volatile unsigned char update_timer_expired = 0;

int main(void) {
    HAL_setup(&update_timer_expired);
    HAL_setup_pwm_input_capture();
    HAL_setup_pwm_output();
    
    HAL_write_string((char*)"\r\nFC Main Starting Up\r\n\0");
    Mpu_6050_initialize();
    
    rollPid.Kp = 0.2; // -0.6 to 0.6
    pitchPid.Kp = 0.2;
    
    //Start Update timer
    while (1) {
        // Wait for update period
        while (!update_timer_expired) {
        }
        update_timer_expired = 0;
        update_number++;
        
        HAL_write_string((char*)".\r\n\0");
        HAL_set_status_light(4, update_number % 2); // toggle stat4 LED on each loop iteration
        
        // Get updated body state from IMU
        ReadAccelerometer(MPU_6050_ADDR, ACC_LSB_2G, &acceleration_data);
        ReadGyroscope(MPU_6050_ADDR, GYRO_LSB_250_DEG_PER_SEC, &gyroscope_data);
        
        float acc_forward = acceleration_data.x_mm_per_sec_squared;
        float acc_right = acceleration_data.y_mm_per_sec_squared;
        float acc_down = acceleration_data.z_mm_per_sec_squared;
        float roll_angle = atan2(acc_down, acc_right);
        float pitch_angle = -1. * atan2(acc_down, acc_forward);
        
        // TODO Get latest RX control input
        float commanded_roll = 0;
        float commanded_pitch = 0;
        float commanded_throttle = 0;
        float commanded_yaw = 0;
        
        float rollError = roll_angle - commanded_roll; // actual - expected
        float pitchError = pitch_angle - commanded_pitch;
        
        // Run control algorithm
        float roll_command = RunPidController(&rollPid, rollError, UPDATE_PERIOD_MS);
        float pitch_command = RunPidController(&pitchPid, pitchError, UPDATE_PERIOD_MS);
        
        control_output_data.pitch_control = roll_command;
        control_output_data.roll_control = pitch_command;
        control_output_data.yaw_control = 0;
        
        // Update PWM controller
        HAL_update_pwm_output(&control_output_data);
    }
}
