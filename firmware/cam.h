#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "sccb_if.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "pico/async_context_freertos.h"

#define USE_100BASE_FX (false)

// #define PICO_PSRAM_CS1 (47)     // QSPI's CS of PSRAM
#define SYS_CLK_IN_KHZ (250000) // 192000 ~ 250000 (if you use sfp, SYS_CLK_KHZ must be just 250000)
#define CAM_BASE_PIN (1)        // GP1 (camera module needs 11pin)
#define PIN_PWM0 (0)            // GP0 (camera's xclk(24MHz))
#define LINEAR_BURST (512)      // IoT SRAM's burst length(in bytes)
#define SFP_HEADER_WORDS (4)    // header length(in words, = sizeof(uint32))

// interfaces
// SCCB IF
#define I2C1_SDA (26)
#define I2C1_SCL (27)

// camera buffer size
// 640x480, RGB565 picture needs 640x480x2 bytes of buffers.
#define IMG_H (256)                                  //(480)
#define IMG_W (256)                                  //(640)
#define PAD_H (256)                                  //(512) // in bytes
#define PAD_W (256)                                  //(512)
#define CAM_FUL_SIZE (IMG_W * IMG_H)                 // VGA size, RGB565(16bit) format
#define CAM_TOTAL_LEN (CAM_FUL_SIZE * 2)             // total length of pictures
#define CAM_TOTAL_FRM (CAM_TOTAL_LEN / CAM_FUL_SIZE) // numbers(or frames) of pictures
#define CAM_PADDED_SIZE_IN_32 (PAD_W * PAD_H / 2)    // in uint32_t[] size

// FreeRTOS Tasks
void vImageProc(void *pvParameters);
// void xMutexInit();

// high layer APIs
void init_cam(uint8_t DEVICE_IS);
void config_cam_buffer();
void start_cam();
void uartout_cam();
void sfp_cam();
void rj45_cam();
void free_cam();
void calc_image();
void set_pwm_freq_kHz(uint32_t freq_khz, uint32_t system_clk_khz, uint8_t gpio_num);
