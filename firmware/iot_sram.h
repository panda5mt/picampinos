#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "pico/multicore.h"

#define IOT_DAT_BASE_PIN    (12)    // IoT SRAM's data-pin {D3,...,D0}
#define IOT_SIG_BASE_PIN    (16)    // IoT SRAM's control pin {SCLK, nCS}
#define LINEAR_BURST        (512)   // IoT SRAM's burst length(in bytes)

// DMA Channel
static uint32_t DMA_IOT_RD_CH  ;
static uint32_t DMA_IOT_WR_CH  ;
// Semaphore for PSRAM handle
static semaphore_t psram_sem; 
// APIs
void iot_sram_init(PIO pio) ;
void iot_sram_set_qpi_mode(PIO pio) ;
void  __time_critical_func(iot_sram_write)(PIO pio, uint32_t *send_data, uint32_t address, 
        uint32_t length_in_byte, uint32_t dma_channel) ;
void  __time_critical_func(*iot_sram_read)(PIO pio, uint32_t *read_data, uint32_t address, 
        uint32_t length_in_byte, uint32_t dma_channel) ;



