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
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "cam.h"
// #include "arithmetic/test_code.h"

#define BOARD_LED (28) // pico's led => 25, self made RP2040brd's led => 28. check hardware/RP2040Board.pdf

static PIO pio_ser_wr = pio1;
static uint sm0;

static void read_i2c_data(i2c_inst_t *i2c)
{
    uint8_t read_buf[256] = {0};
    uint32_t x = 0, y = 0;
    uint8_t tmp;
    printf("read sfp start...\r\n");

    // sfp's addr = 0xA0
    i2c_read_blocking(i2c, (0xA0 >> 1), read_buf, 256, false);

    printf("read sfp end...\r\n");
    printf(" 0x50 0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef\r\n");
    for (y = 0; y < 16; y++)
    {
        printf(" %02x: ", y << 4);
        for (x = 0; x < 16; x++)
        {
            printf("%02x ", read_buf[(y << 4) + x]);
        }
        printf("   ");
        for (x = 0; x < 16; x++)
        {
            tmp = read_buf[(y << 4) + x];
            if (tmp >= 0x20 && tmp <= 0x7E)
            {
                printf("%c", tmp);
            }
            else
            {
                printf(".");
            }
        }
        printf("\r\n");
    }
}

bool setup()
{
    bool OC_INIT = false;
    vreg_set_voltage(VREG_VOLTAGE_1_40);
    // system init
    stdio_init_all();
    sleep_ms(300);
    OC_INIT = set_sys_clock_khz(SYS_CLK_IN_KHZ, true);
    sleep_ms(600);
    setup_default_uart();

    // INIT LED
    gpio_init(BOARD_LED);
    gpio_set_dir(BOARD_LED, GPIO_OUT);
    for (int i = 0; i < 5; i++)
    { // blink LED
        gpio_put(BOARD_LED, 1);
        busy_wait_ms(100);
        gpio_put(BOARD_LED, 0);
        busy_wait_ms(100);
    }
    return OC_INIT;
}

int main()
{

    bool OC_OK = false;
    OC_OK = setup();

    if (OC_OK)
    {
        printf("clock init Okay at %dkHz\r\n", SYS_CLK_IN_KHZ);
        sleep_ms(1000);
    }
    else
    {
        printf("clock init failed.\r\n");
        sleep_ms(1000);
    }

    // fft_test();
    // while(1);

    init_cam(DEV_OV5642);

    // check SFP.
    // call 'read_i2c_data()' after 'init_cam()' because i2c hardware is not initialized before 'init_cam()'
    i2c_inst_t *i2c = i2c1;
    read_i2c_data(i2c);
    config_cam_buffer(); // config buffer
    start_cam();         // start streaming

    // data via USB-UART(ASCII)
    // see also 'matlab/readrgb.m'
    // while(true) {
    //     uartout_cam();
    // }

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

    // using SFP module
    // see also 'matlab/receive_udp.m'
    // To Use SFP, CHECK 'USE_EZSPI_SLAVE' is (false)
    // AND 'USE_100BASE_FX' is (true) in 'cam.h'
    multicore_launch_core1(sfp_cam);
    while (true)
    {
    };

    // end
    free_cam();
    while (true)
    {
    };
}
