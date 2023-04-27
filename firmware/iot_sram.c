#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "iot_sram.pio.h"
#include "iot_sram.h"
#include "hardware/dma.h"

uint32_t sm_iot;
uint32_t offset_iot;
volatile bool psram_initialized = false;

void iot_sram_init(PIO pio) {

    if(true == psram_initialized) {
        return;
    }
    else {
        psram_initialized = true;
    }

    // Initialize IoT SRAM
    offset_iot = pio_add_program(pio, &iot_sram_program);
    sm_iot = 2;//pio_claim_unused_sm(pio, true);
    //printf("psram:pio = %d, sm = %d, offset=%d\r\n",(uint32_t)pio, sm_iot, offset_iot);

    iot_sram_program_init(pio, sm_iot, offset_iot, IOT_DAT_BASE_PIN, 4, IOT_SIG_BASE_PIN, 2); // : total 6 pins
    iot_sram_set_qpi_mode(pio);
    
    DMA_IOT_RD_CH  = dma_claim_unused_channel(true);
    DMA_IOT_WR_CH  = dma_claim_unused_channel(true);

}

/// IoT SRAM:APS1604M-3SQR (AP Memory)
void  iot_sram_set_qpi_mode(PIO pio) {
    // ----------send reset
    pio_sm_put_blocking(pio, sm_iot, 8-1);      // x=8
    pio_sm_put_blocking(pio, sm_iot, 0);        // y=0
    {
        pio_sm_put_blocking(pio, sm_iot, 0xeffeeffe); 
    }
    pio_sm_put_blocking(pio, sm_iot, 8-1);      // x=8
    pio_sm_put_blocking(pio, sm_iot, 0);        // y=0    
    {
        pio_sm_put_blocking(pio, sm_iot, 0xfeeffeef);  
    }
    sleep_ms(1);
    // ----------send qpi enable mode
    pio_sm_put_blocking(pio, sm_iot, 8-1);      // x=8
    pio_sm_put_blocking(pio, sm_iot, 0);        // y=0    
    {
        pio_sm_put_blocking(pio, sm_iot, 0xeeffefef);  
    }
    sleep_ms(1);
}

void  __time_critical_func(iot_sram_write)(PIO pio, uint32_t *send_data, uint32_t address, 
        uint32_t length_in_byte, uint32_t dma_channel) {

    uint32_t *b;
    uint32_t num_of_tran_byte; // number of trans at once.(upto LINEAR_BURST bytes)
    b = send_data;
    dma_channel_config c = dma_channel_get_default_config(dma_channel);    
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm_iot, true));
    pio_sm_set_enabled(pio, sm_iot, true);

    while (true) {

        // IoT SRAM needs refresh between writing LINEAR_BURST bytes
        if (length_in_byte > LINEAR_BURST) {
            num_of_tran_byte = LINEAR_BURST;
            length_in_byte = length_in_byte - LINEAR_BURST;
        } else {
            num_of_tran_byte = length_in_byte;
            length_in_byte = 0;
        }
    
        dma_channel_configure(dma_channel, &c,
            &pio->txf[sm_iot],      // Destination pointer
            b,                      // Source pointer
            num_of_tran_byte/4,     // Number of transfers
            false                   // Do not Start yet.
        );

        pio_sm_put_blocking(pio, sm_iot, 8+(num_of_tran_byte*2)-1);         // x=8
        pio_sm_put_blocking(pio, sm_iot, 0);                                // y=0 
        pio_sm_put_blocking(pio, sm_iot, 0x38000000 | address);
    
        // Start DMA
        dma_start_channel_mask(1u << dma_channel);
        dma_channel_wait_for_finish_blocking(dma_channel);

        if (0 == length_in_byte) break;
        else {
            b = b + LINEAR_BURST/(sizeof(uint32_t)); // b += LINEAR_BURST / (sizeof(uint32_t))
            address = address + LINEAR_BURST;
        }
    }
    return;
}

void  __time_critical_func(*iot_sram_read)(PIO pio, uint32_t *read_data, uint32_t address, 
        uint32_t length_in_byte, uint32_t dma_channel) {
   
    uint32_t *b;
    b = read_data;
    uint32_t num_of_tran_byte = 0; // number of trans at once.(upto LINEAR_BURST bytes)
    dma_channel_config c = dma_channel_get_default_config(dma_channel);    
    channel_config_set_read_increment(&c, false);
    channel_config_set_write_increment(&c, true);
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm_iot, false));
    pio_sm_set_enabled(pio, sm_iot, true);

    while (true) {

        // IoT SRAM needs refresh between writing LINEAR_BURST bytes
        if (length_in_byte > LINEAR_BURST) {
            num_of_tran_byte = LINEAR_BURST;
            length_in_byte = length_in_byte - LINEAR_BURST;
        } else {
            num_of_tran_byte = length_in_byte;
            length_in_byte = 0;
        }
       
        dma_channel_configure(dma_channel, &c,
            b,                      // Destination pointer
            &pio->rxf[sm_iot],          // Source pointer
            num_of_tran_byte/4,     // Number of transfers
            false                   // do not start yet.
        );

        pio_sm_set_enabled(pio, sm_iot, true);
        pio_sm_put_blocking(pio, sm_iot, 8-1);                          // x counter: comm + addr
        pio_sm_put_blocking(pio, sm_iot, (num_of_tran_byte * 2)-1);     // y counter: up to 512byte    

        pio_sm_put_blocking(pio, sm_iot, 0xEB000000 | address);         // send write command + address
        
        // start DMA
        dma_start_channel_mask(1u << dma_channel);
        dma_channel_wait_for_finish_blocking(dma_channel);
       
        if (0 >= length_in_byte) break;
        else {
            b = b + LINEAR_BURST/4; // b += LINEAR_BURST / (sizeof(uint32_t))
            address = address + LINEAR_BURST;
        }
    }

    return (void *)read_data;
}


