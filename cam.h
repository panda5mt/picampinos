#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "sccb_if.h"

// USE SPI Slave?
#define USE_EZSPI_SLAVE     (true)

#define SYS_CLK_KHZ         (300000)// 192000 ~ 264000
#define CAM_BASE_PIN        (1)     // GP1 (camera module needs 11pin)
#define PIN_PWM0            (0)     // GP0 (camera's xclk(24MHz))
// #define IOT_DAT_BASE_PIN    (16)    // IoT SRAM's data-pin(D0~D3)
// #define IOT_SIG_BASE_PIN    (20)    // IoT SRAM's control pin (nCS, SCLK)
#define LINEAR_BURST        (512)   // IoT SRAM's burst length(in bytes)

// interfaces
// SCCB IF
#define I2C1_SDA    (26)
#define I2C1_SCL    (27)

// camera buffer size
// 640x480, RGB565 picture needs 640x480x2 bytes of buffers.
// but RP2040 has no capacity such as huge buffers.
// so, using DMA and IoT SRAM and lesser buffer to store them.
#define BLOCK           (80)                // fixed: 80
#define CAM_FUL_SIZE    (640 * 480 * 2)     // VGA size, RGB565(16bit) format
#define CAM_BUF_SIZE    (640 * BLOCK * 2)   // in bytes
#define CAM_BUF_HALF    (CAM_BUF_SIZE / 2)  // in bytes

// high layer APIs
void init_cam(uint8_t DEVICE_IS);
void config_cam_buffer();
void start_cam();
void uartout_cam();
void uartout_bin_cam();
void spiout_cam();
void free_cam();

