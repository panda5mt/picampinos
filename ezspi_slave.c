#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "ezspi_slave.pio.h"
#include "ezspi_slave.h"

PIO pio_spi = pio1; // same PIO with pio_iot
uint32_t sm_spi;    // ezspi slave's state machines
uint32_t offset_spi;
volatile bool spi_initialized = false;


/// SPI
void init_spi_slave() {
    // SPI1 Slave
    // SPI1_RX=GP12, SPI1_CSn=GP13, SPI1_SCK=GP14, SPI1_TX=GP15
    // this is not HW-SPI but pseudo-SPI Slave using PIO. 
    // because RP2040's SPI Slave(PL022 by arm Limited)is not suitable. 
    if(true == spi_initialized) {
        return;
    }
    else {
        spi_initialized = true;
    }
    offset_spi = pio_add_program(pio_spi, &ezspi_slave_program);
    sm_spi = pio_claim_unused_sm(pio_spi, true);
    ezspi_slave_program_init(pio_spi, sm_spi, offset_spi, SPI1S_RX, 3, SPI1S_TX, 1); // in:MSB-> {CLK, nCS, RX} <-LSB, out: TX
    
    return;
}

/// SPI
void deinit_spi_slave() {
    // SPI1 Slave
    // SPI1_RX=GP12, SPI1_CSn=GP13, SPI1_SCK=GP14, SPI1_TX=GP15
    // this is not HW-SPI but pseudo-SPI Slave using PIO. 
    // because RP2040's SPI Slave(PL022 by arm Limited)is not suitable. 
    if(false == spi_initialized) {
        return;
    }
    else {
        spi_initialized = false;
    }
    // stop Spi
    pio_sm_set_enabled(pio_spi, sm_spi, false);
    pio_sm_clear_fifos(pio_spi, sm_spi);
    pio_sm_unclaim(pio_spi,sm_spi);
    pio_remove_program(pio_spi,&ezspi_slave_program, offset_spi);
   
    return;
}

uint32_t write_read_word_spi_slave(uint32_t out_buf) {
    uint32_t resp;
    pio_sm_put_blocking(pio_spi, sm_spi, 4-1);       // send receive one word.
    pio_sm_put_blocking(pio_spi, sm_spi, out_buf);   // put data
    resp = pio_sm_get_blocking(pio_spi, sm_spi);     // get data
}

void write_read_blocking_spi_slave(uint32_t* out_buf, uint32_t* in_buf, uint32_t size_in_byte) {

    uint32_t size_in_word = size_in_byte / sizeof(uint32_t);
    uint32_t dummy;
    // send pio how many size we want to send and recv  
    pio_sm_put_blocking(pio_spi, sm_spi, (size_in_byte - 1));       // size_in_byte

    for(uint32_t j = 0 ; j < size_in_word ; j++) {
        pio_sm_put_blocking(pio_spi, sm_spi, out_buf[j]);   // put data
        in_buf[j] = pio_sm_get_blocking(pio_spi, sm_spi);   // get data
    }
    
}
void write_blocking_spi_slave(uint32_t* out_buf, uint32_t size_in_byte) {

    uint32_t size_in_word = size_in_byte / sizeof(uint32_t);
    uint32_t dummy;
    // send pio how many size we want to send and recv  
    pio_sm_put_blocking(pio_spi, sm_spi, (size_in_byte - 1));       // size_in_byte

    for(uint32_t j = 0 ; j < size_in_word ; j++) {
        pio_sm_put_blocking(pio_spi, sm_spi, out_buf[j]);   // put data
        dummy = pio_sm_get_blocking(pio_spi, sm_spi);       // get data
    }
    
}

void printbuf(uint8_t buf[], size_t len) {
    int i;
    for (i = 0; i < len; ++i) {
        if (i % 16 == 15)
            printf("%02x\n", buf[i]);
        else
            printf("%02x ", buf[i]);
    }

    // append trailing newline if there isn't one
    if (i % 16) {
        putchar('\n');
    }
}