#include "hardware/gpio.h"
#include "hardware/pio.h"

// SPI Slave
// you can change base(LSB) pin but pin order:(MSB)TX,SCK,CSn,RX(LSB)
#define SPI1S_RX    (18)
#define SPI1S_CSn   (19)
#define SPI1S_SCK   (20)
#define SPI1S_TX    (21)

void init_spi_slave(PIO pio) ;
void deinit_spi_slave(PIO pio) ;
void write_word_spi_slave(PIO pio, uint32_t out_buf) ;
void write_blocking_spi_slave(PIO pio, uint32_t* out_buf, uint32_t size_in_byte) ;
// void write_read_blocking_spi_slave(PIO pio, uint32_t* out_buf, uint32_t* in_buf, uint32_t size_in_byte) ;

