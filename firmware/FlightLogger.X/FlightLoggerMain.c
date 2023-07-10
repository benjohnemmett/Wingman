#include "ATMega4809/HAL_ATMega4809.h"

int main(void) {

    HAL_hardware_setup();
    HAL_flash_status_lights();
    
    HAL_print_string((char*)"\r\nFlightLogger Main Starting Up\r\n\0");
        
    while (1) {
        HAL_print_string((char*)" - Insert log data here\r\n\0");
    }
}
