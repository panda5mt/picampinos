#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "pico/binary_info.h"
#include "cam.h"

#define USE_CAMERA_SYSTEM   (1)     // 1: USE CAMERA & IoT SRAM
#define BOARD_LED           (25)

#if USE_ADC
#define ADC0_PIN            (26)    // ADC0 input pin(gp26)
#define ADC1_PIN            (27)    // ADC1 input pin(gp27)
#define ADC2_PIN            (28)    // ADC1 input pin(gp28)
#endif // USE_ADC

void setup() {
    // system init
    set_sys_clock_khz(SYS_CLK_KHZ, true);
    stdio_init_all();

    // INIT LED
    gpio_init(BOARD_LED);
    gpio_set_dir(BOARD_LED, GPIO_OUT);
    gpio_put(BOARD_LED, 1);
    //gpio_pull_up(BOARD_LED);

}


int main() {

    setup();

    init_cam(DEV_OV5642);
   
    config_cam_buffer();    // config buffer
    capture_cam();          // get still
    uartout_cam();          // data from USB-UART

    // need more still?
    
    /*
    config_cam_buffer();
    capture_cam();
    uartout_cam();
    */
    
    // end
    while (true);
}


