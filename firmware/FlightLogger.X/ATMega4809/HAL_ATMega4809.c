#include "HAL_ATMega4809.h"
#include <util/delay.h>
#include "../../Libraries/atmega4809-stuff/UART.X/uart.h"

void HAL_hardware_setup() {
    // Set CPU clock divider to 1
    CCP = CCP_IOREG_gc; //Configuration Change Protection
    CLKCTRL.MCLKCTRLB = 0; //Clock Div = 1
    uart0_init(9600);
    
    // Setup Status Lights
    __CONFIGURE_PIN_AS_OUTPUT__(STAT_LED_1_PORT, STAT_LED_1_PIN);
    __CONFIGURE_PIN_AS_OUTPUT__(STAT_LED_2_PORT, STAT_LED_2_PIN);
    __CONFIGURE_PIN_AS_OUTPUT__(STAT_LED_3_PORT, STAT_LED_3_PIN);
    __CONFIGURE_PIN_AS_OUTPUT__(STAT_LED_4_PORT, STAT_LED_4_PIN);
    STAT1_OFF();
    STAT2_OFF();
    STAT3_OFF();
    STAT4_OFF();
}

void HAL_print_string(char* string) {
    uart0_send_string(string);
}

void HAL_flash_status_lights() {
    STAT1_ON();
    _delay_ms(300);
    STAT1_OFF();
    STAT2_ON();
    _delay_ms(300);
    STAT2_OFF();
    STAT3_ON();
    _delay_ms(300);
    STAT3_OFF();
    STAT4_ON();
    _delay_ms(300);
    STAT4_OFF();
}
