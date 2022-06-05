
#include "hardware/gpio.h"
#include "hardware/pio.h"

// SPI Slave
#define SPI1S_RX    (12)
#define SPI1S_CSn   (13)
#define SPI1S_SCK   (14)
#define SPI1S_TX    (15)

void init_spi_slave(PIO pio) ;
void deinit_spi_slave(PIO pio) ;
void write_word_spi_slave(PIO pio, uint32_t out_buf) ;
void write_blocking_spi_slave(PIO pio, uint32_t* out_buf, uint32_t size_in_byte) ;
// void write_read_blocking_spi_slave(PIO pio, uint32_t* out_buf, uint32_t* in_buf, uint32_t size_in_byte) ;