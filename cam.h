#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "sccb_if.h"

#define SYS_CLK_KHZ         (264000)// 192000 ~ 264000
#define CAM_BASE_PIN        (1)     // GP1 (camera module needs 11pin)
#define PIN_PWM0            (0)     // GP0 (camera's xclk(24MHz))
#define IOT_DAT_BASE_PIN    (14)    // IoT SRAM's data-pin(D0~D3)
#define IOT_SIG_BASE_PIN    (18)    // IoT SRAM's control pin (nCS, SCLK)
#define LINEAR_BURST        (512)   // IoT SRAM's burst length(in bytes)

// camera buffer size
// 640x480, RGB565 picture needs 640x480x2 bytes of buffers.
// but RP2040 has no capacity such as huge buffers.
// so, using DMA and IoT SRAM and lesser buffer to store them.
#define BLOCK           (160)               // fixed: 160
#define CAM_FUL_SIZE    (640 * 480 * 2)     // VGA size, RGB565(16bit) format
#define CAM_BUF_SIZE    (640 * BLOCK * 2)   // in bytes
#define CAM_BUF_HALF    (CAM_BUF_SIZE / 2)  // in bytes


void iot_sram_init(PIO pio, uint32_t sm);
void iot_sram_write(PIO pio, uint32_t sm, uint32_t *send_data, uint32_t address, uint32_t length_in_byte, uint32_t dma_channel);
void *iot_sram_read(PIO pio, uint32_t sm, uint32_t *read_data, uint32_t address, uint32_t length_in_byte, uint32_t dma_channel);
void init_cam(uint8_t DEVICE_IS);
void config_cam_buffer();
void capture_cam();
void uartout_cam();
void free_cam();

