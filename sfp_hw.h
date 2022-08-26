#include "hardware/pio.h"
#include "ser_100base_fx.pio.h"
#include "tbl_8b10b.h"
#include "udp.h"    

#define HW_PINNUM_SFP0_TXD  (22)

void sfp_hw_init(PIO pio) ;
void sfp_send(void* str, uint16_t len) ;