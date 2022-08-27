#include "hardware/pio.h"
#include "ser_100base_fx.pio.h"
#include "tbl_8b10b.h"
#include "udp.h"    

#define HW_PINNUM_SFP0_TXD  (22)

void sfp_hw_init(PIO pio) ;
void sfp_send(void* str, uint16_t len) ;
void sfp_send_with_header(uint32_t header1, uint32_t header2, uint32_t header3, uint32_t header4, void* str, uint16_t len) ;
void sfp_test(void);