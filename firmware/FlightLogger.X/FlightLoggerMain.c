#include "../Libraries/atmega4809-stuff/I2C.X/i2c.h"
#include "../Libraries/atmega4809-stuff/I2C.X/i2c_interface.h"
#include "../HAL_Common.X/FlightControllerCommonTypes.h"
#include "../HAL_Common.X/HAL_Functions.h"

I2cFunctions i2c_functions;
volatile PwmInputCapture pwm_input_capture;
volatile PwmOutputData pwm_output_data;

int main(void) {
    //
    i2c_functions.f_I2cInitialize = I2cInitialize;
    i2c_functions.f_I2cWriteByte = I2cWriteByte;
    i2c_functions.f_I2cWriteBytes = I2cWriteBytes;
    i2c_functions.f_I2cSendStart = I2cSendStart;
    i2c_functions.f_I2cWrite = I2cWrite;
    i2c_functions.f_I2cRead = I2cRead;
    i2c_functions.f_I2cSendStop = I2cSendStop;
    
    platform_specific_setup(); // TODO Maybe rename these HAL functions to HAL_hardware_setup() and HAL_write()
    
    platform_specific_write_string((char*)"\r\nFlightLogger Main 1.0 Starting Up\r\n\0");
        
    while (1) {
    }
}
