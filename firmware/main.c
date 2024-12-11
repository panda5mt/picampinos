// PICAMPINOS - Camera IF using RP2350
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

#include <stdio.h>
#include "hardware/clocks.h"
#include "hardware/vreg.h"

#include "cam.h"
#include "hwinit.h"
#include "eth.h"
#include "arithmetic/fft_helper.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "pico/async_context_freertos.h"

#define BOARD_LED (25) // 28 // pico's led => 25, self made RP2350brd's led => 28. check hardware/RP2350Board.pdf

// FreeRTOS Tasks
TaskHandle_t rj45Handle;
TaskHandle_t rxHandle;
TaskHandle_t imageHandle;

void vRJ45Task(void *pvParameters);
// FreeRTOS Tasks: End

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

    stdio_init_all();
    bool OC_INIT = false;
#if PICO_RP2040
    vreg_set_voltage(VREG_VOLTAGE_1_20);
#else
    vreg_set_voltage(VREG_VOLTAGE_1_25);
#endif
    // system init
    stdio_init_all();
    sleep_ms(1000);
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

    // init_cam(DEV_OV5642);

#if USE_100BASE_FX
    // check SFP.
    // call 'read_i2c_data()' after 'init_cam()' because i2c hardware is not initialized before 'init_cam()'
    i2c_inst_t *i2c = i2c1;
    // read_i2c_data(i2c);
#endif

    init_cam(DEV_OV5642);
    config_cam_buffer(); // config buffer
    start_cam();         // start streaming
    printf("[CAM INIT]\n");
    eth_init();
    printf("[BOOT]\r\n");

    UBaseType_t uxCoreAffinityMask;
    xTaskCreate(vRJ45Task, "Eth Task", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 1, &rj45Handle);
    xTaskCreate(vLaunchRxFunc, "Rx Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, &rxHandle);
    xTaskCreate(vImageProc, "Image Task", configMINIMAL_STACK_SIZE * 5, NULL, tskIDLE_PRIORITY + 2, &imageHandle);
    // 処理タスクの作成
    xTaskCreate(vProcessingFFTTask, "ProcessingTask", configMINIMAL_STACK_SIZE + 100, NULL, 2, &FFTTaskHandle);
    // xTaskCreate(rftfcol_task, "Rftfcol_Task_1", 2048, NULL, 1, &rftfcol_task_handle);

    // xMutexInit();
    uxCoreAffinityMask = ((1 << 0)); // Core0
    vTaskCoreAffinitySet(rj45Handle, uxCoreAffinityMask);
    vTaskCoreAffinitySet(FFTTaskHandle, uxCoreAffinityMask);

    uxCoreAffinityMask = ((1 << 1)); // Core1
    vTaskCoreAffinitySet(rxHandle, uxCoreAffinityMask);
    vTaskCoreAffinitySet(imageHandle, uxCoreAffinityMask);

    //   FreeRTOSのスケジューラを開始
    vTaskStartScheduler();

    // // data via USB-UART(ASCII)
    // // see also 'matlab/readrgb.m'
    // while (true)
    // {
    //     uartout_cam();
    // }

    // data via USB-UART(binary)
    // see also 'matlab/comm_uart_bin.m'
    /*
    while(true) {
        uartout_bin_cam();
    }
    */

    // using SFP module
    // see also 'matlab/receive_udp.m'
    // To Use SFP, CHECK 'USE_EZSPI_SLAVE' is (false)
    // AND 'USE_100BASE_FX' is (true) in 'cam.h'
    // multicore_launch_core1(sfp_cam);
    while (true)
        ;

    // free_cam();
    while (true)
        ;
}

void vRJ45Task(void *pvParameters)
{
    printf("RJ45Task - Running on Core: %d\n", get_core_num()); // 現在のコア番号を表示
    while (1)
    {
        eth_main();
        rj45_cam();
    }
}
