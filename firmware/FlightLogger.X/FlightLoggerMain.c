
#include "../Libraries/atmega4809-stuff/I2C.X/i2c.h"
#include "../Libraries/atmega4809-stuff/I2C.X/i2c_interface.h"
#include "../HAL_Common.X/FlightControllerCommonTypes.h"
#include "../HAL_Common.X/HAL_Functions.h"
#include "../Libraries/atmega4809-stuff/MPU-6050.X/mpu_6050.h"

I2cFunctions i2c_functions;
GyroscopeData gyroscope_data;
AccelerationData acceleration_data;
volatile PwmInputCapture pwm_input_capture;
volatile PwmOutputData pwm_output_data;

int main(void) {
    i2c_functions.f_I2cInitialize = I2cInitialize;
    i2c_functions.f_I2cWriteByte = I2cWriteByte;
    i2c_functions.f_I2cWriteBytes = I2cWriteBytes;
    i2c_functions.f_I2cSendStart = I2cSendStart;
    i2c_functions.f_I2cWrite = I2cWrite;
    i2c_functions.f_I2cRead = I2cRead;
    i2c_functions.f_I2cSendStop = I2cSendStop;
    
    HAL_setup();
    HAL_write_string((char*)"\r\nFlightLogger Main 0.2 Starting Up\r\n\0");
    
    I2cInitialize();    
    Mpu_6050_initialize(&i2c_functions);
    
    HAL_write_string((char*)"ACC_X,ACC_Y,ACC_Z\r\n\0");
    
    while (1) {
        ReadAccelerometer(&i2c_functions, MPU_6050_ADDR, ACC_LSB_2G, &acceleration_data);
        ReadGyroscope(&i2c_functions, MPU_6050_ADDR, GYRO_LSB_250_DEG_PER_SEC, &gyroscope_data);
        
        uart0_print_s16(((signed int)(acceleration_data.x_mm_per_sec_squared*1000)));
        HAL_write_string((char*)",\0");
        uart0_print_s16(((signed int)(acceleration_data.y_mm_per_sec_squared*1000)));
        HAL_write_string((char*)",\0");
        uart0_print_s16(((signed int)(acceleration_data.z_mm_per_sec_squared*1000)));
        HAL_write_string((char*)",\0");
        uart0_print_s16(gyroscope_data.x_deg_per_second);
        HAL_write_string((char*)",\0");
        uart0_print_s16(gyroscope_data.y_deg_per_second);
        HAL_write_string((char*)",\0");
        uart0_print_s16(gyroscope_data.z_deg_per_second);
        uart0_send_string((char*)"\r\n\0");
        
        HAL_sleep_ms(100);
    }
}
