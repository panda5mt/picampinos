// PICAMPINOS - Camera IF using RP2040
// this project communicates OV5642(or parallel 8-bit DVP-IF Image sensor with SCCB control) using Raspberry Pi PICO
// Need: A Raspberry Pi PICO, An IoT SRAM(APS1604M-3SQR or compatible), A DVP Camera(OV5642)
// Connection:
// ----- with OV Camera -----
// You can reverse bits (GP1:GP8=D2:D9 -> GP1:GP8=D9:D2). search for keywords 'bit reverse' in 'picampinos.pio'
// (PICO <-> OV5642/OV2640)
// GP0 ---> XCLK(24MHz Clock IN)
// GP1 <--- D2 
// GP2 <--- D3
// GP3 <--- D4
// GP4 <--- D5
// GP5 <--- D6
// GP6 <--- D7
// GP7 <--- D8
// GP8 <--- D9
// GP9 <--- VSYNC
// GP10 <-- HREF(HSYNC)
// GP11 <-- PCLK(Pixel Clock OUT)

// GP12~GP15 : reserved for SPI. (SPI: See also ezspi_slave.c if you want to use.)

// GP26 --> SDA(SCCB DATA/I2C)
// GP27 --> SCL(SCCB CLK/I2C)

// ----- with IoT SRAM -----
// (PICO <-> APS1604M)
// GP16 <-> D0
// GP17 <-> D1
// GP18 <-> D2
// GP19 <-> D3
// GP20 --> xCS
// GP21 --> SCLK

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "cam.h"

#define BOARD_LED           (28) // pico => 25, self made RP2040brd => 28. check hardware/RP2040Board.pdf 

void setup() {
    vreg_set_voltage(VREG_VOLTAGE_1_30);
    // system init
    stdio_init_all();
    set_sys_clock_khz(SYS_CLK_KHZ, true);
    setup_default_uart();

    // INIT LED
    gpio_init(BOARD_LED);
    gpio_set_dir(BOARD_LED, GPIO_OUT);
    for(int i=0 ; i < 5; i++) {
        gpio_put(BOARD_LED, 1);
        busy_wait_ms(100);
        gpio_put(BOARD_LED, 0);
        busy_wait_ms(100);
    }
    
    //gpio_pull_up(BOARD_LED);

}


int main() {

    setup();

    init_cam(DEV_OV5642);
   
    config_cam_buffer();    // config buffer
    start_cam();            // start streaming

    
    // data via USB-UART(ASCII)
    // see also 'matlab/readrgb.m'
    
    while(true) {
        uartout_cam();          
    }
    

    // data via USB-UART(binary) 
    // see also 'matlab/comm_uart_bin.m'
    /*    
    while(true) {
        uartout_bin_cam();      
    }                           
    */
    // you have Raspberry Pi 3/4? and you have MATLAB?
    // you can use SPI + RPi + MATLAB
    // and CHECK 'USE_EZSPI_SLAVE' is (true) in 'cam.h'
    // See also 'matlab/comm_raspi_spi.m'
    /*
    while(true) {
        spiout_cam();
        printf("OK\r\n");
    }
    */

    // end
    free_cam();

    while (true);
}


