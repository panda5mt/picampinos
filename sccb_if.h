#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

/*******************************************************************************
 * Function Declarations
 */

#define DEV_OV5642  (1)
#define DEV_OV2640  (2)

void sccb_init(uint8_t device_is);

int32_t reg_write(i2c_inst_t *i2c, 
                const uint32_t addr, 
                uint8_t *buf,
                const uint8_t nbytes);

int32_t reg_read(   i2c_inst_t *i2c,
                const uint32_t addr,
                uint8_t *buf,
                const uint8_t nbytes);

