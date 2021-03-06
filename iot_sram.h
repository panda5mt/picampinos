#include "hardware/gpio.h"
#include "hardware/pio.h"

#define IOT_DAT_BASE_PIN    (16)    // IoT SRAM's data-pin(D0~D3)
#define IOT_SIG_BASE_PIN    (20)    // IoT SRAM's control pin (nCS, SCLK)
#define LINEAR_BURST        (512)   // IoT SRAM's burst length(in bytes)

// DMA Channel
uint32_t DMA_IOT_RD_CH  ;
uint32_t DMA_IOT_WR_CH  ;

// APIs
void iot_sram_init(PIO pio) ;
void iot_sram_set_qpi_mode(PIO pio) ;
void iot_sram_write(PIO pio, uint32_t *send_data, uint32_t address, 
        uint32_t length_in_byte, uint32_t dma_channel) ;
void *iot_sram_read(PIO pio, uint32_t *read_data, uint32_t address, 
        uint32_t length_in_byte, uint32_t dma_channel) ;
