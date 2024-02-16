
#include <stdio.h>
//TODO The I2C stuff should go through the HAL
#include "../Libraries/atmega4809-stuff/I2C.X/i2c.h"
#include "../Libraries/atmega4809-stuff/I2C.X/i2c_interface.h"
#include "../HAL_Common.X/FlightControllerCommonTypes.h"
#include "../HAL_ATMega4809.X/HAL_ATMega4809.h"
#include "../Libraries/atmega4809-stuff/MPU-6050.X/mpu_6050.h"
#include "../Libraries/atmega4809-stuff/BMP390.X/bmp390.h"

#define F_CPU 20000000
#define BMP390_ADDRESS 0b01110110

I2cFunctions i2c_functions;
GyroscopeData gyroscope_data;
AccelerationData acceleration_data;
volatile PwmInputCapture pwm_input_capture;
volatile PwmOutputData pwm_output_data;
volatile unsigned char update_timer_expired = 0;
char string_buffer[120];

int main(void) {
    HAL_setup(&update_timer_expired);
    
    HAL_write_string((char*)"\r\nFlightLogger Main 0.4 Starting Up\r\n\0");
    HAL_write_string((char*)"acc_x,acc_y,acc_z,gyr_x,gyr_y,gyr_z,temp_c,pressure,pwm_1,pwm_2,pwm_3,pwm_4\r\n\0");
    
    HAL_setup_pwm_input_capture();
    
    i2c_functions.f_I2cInitialize = I2cInitialize;
    i2c_functions.f_I2cWriteByte = I2cWriteByte;
    i2c_functions.f_I2cWriteBytes = I2cWriteBytes;
    i2c_functions.f_I2cReadByte = I2cReadByte;
    i2c_functions.f_I2cReadBytes = I2cReadBytes;
    i2c_functions.f_I2cSendStart = I2cSendStart;
    i2c_functions.f_I2cWrite = I2cWrite;
    i2c_functions.f_I2cRead = I2cRead;
    i2c_functions.f_I2cSendStop = I2cSendStop;
    
    I2cInitialize();
    Mpu_6050_initialize(&i2c_functions);
    
    Bmp390CalibrationData calibration_data;
    Bmp390Initialize(&i2c_functions, BMP390_ADDRESS, &calibration_data);

    Bmp390SetIirFilterCoefficient(&i2c_functions, BMP390_ADDRESS, BMP390_IIR_FILTER_COEF_3_bm);
    Bmp390SetOverSampleRates(&i2c_functions, BMP390_ADDRESS, BMP390_OSR_PRESSURE_8x_bm & BMP390_OSR_TEMPERATURE_1x_bm);
    Bmp390SetOutputDataRate(&i2c_functions, BMP390_ADDRESS, BMP390_ODR_6p25_HZ);
        
    while (1) {
        ReadAccelerometer(&i2c_functions, MPU_6050_ADDR, ACC_LSB_2G, &acceleration_data);
        ReadGyroscope(&i2c_functions, MPU_6050_ADDR, GYRO_LSB_250_DEG_PER_SEC, &gyroscope_data);
        
        float temp_c = Bmp390ReadTemperatureInC(&i2c_functions, BMP390_ADDRESS, &calibration_data);
        //float temp_f = temp_c * 1.8 + 32;
        float pressure_pa = Bmp390ReadPressureInPa(&i2c_functions, BMP390_ADDRESS, &calibration_data);

        sprintf(string_buffer, "%g,%g,%g,",
                acceleration_data.x_mm_per_sec_squared,
                acceleration_data.y_mm_per_sec_squared,
                acceleration_data.z_mm_per_sec_squared);
        HAL_write_string(string_buffer);
        
        sprintf(string_buffer, "%i,%i,%i,",
                gyroscope_data.x_deg_per_second,
                gyroscope_data.y_deg_per_second,
                gyroscope_data.z_deg_per_second);
        HAL_write_string(string_buffer);
        
        sprintf(string_buffer, "%g,%g,",
                temp_c,
                pressure_pa);
        HAL_write_string(string_buffer);
        
        sprintf(string_buffer, "%i,%i,%i,%i\r\n",
                pwm_input_capture.ch1_pulse_width_us,
                pwm_input_capture.ch2_pulse_width_us,
                pwm_input_capture.ch3_pulse_width_us,
                pwm_input_capture.ch4_pulse_width_us
                );
        
        HAL_write_string(string_buffer);
        
        HAL_sleep_ms(100);
    }
}
