#include <stdio.h>
#include "pico/stdlib.h"
#include "sccb_if.h"



/*******************************************************************************
 * Function Definitions
 */
void sccb_init(uint8_t device_is, const uint32_t sda_pin, const uint32_t scl_pin) {


    uint8_t CAM_ADDR=(0x78>>1); // default: OV5642

    // Ports
    i2c_inst_t *i2c = i2c1;

    // Buffer to store raw reads
    uint8_t data[6];

    //Initialize I2C port at 100 kHz
    i2c_init(i2c, 100 * 1000);

    // Initialize I2C pins
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    uint8_t sccb_dat[3];
    switch(device_is) {

    case DEV_OV2640:
        CAM_ADDR=(0x60>>1);
       
        sccb_dat[0] = 0xff ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Device control register list Table 12 */
        sccb_dat[0] = 0x2c ; sccb_dat[1] = 0xff ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x2e ; sccb_dat[1] = 0xdf ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0xff ; sccb_dat[1] = 0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Device control register list Table 13 */
        sccb_dat[0] = 0x3c ; sccb_dat[1] = 0x32 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x11 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Clock Rate Control                    */
        sccb_dat[0] = 0x09 ; sccb_dat[1] = 0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Common control 2                      */
        sccb_dat[0] = 0x04 ; sccb_dat[1] = 0x28 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Mirror                                */
        sccb_dat[0] = 0x13 ; sccb_dat[1] = 0xe5 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Common control 8                      */
        sccb_dat[0] = 0x14 ; sccb_dat[1] = 0x48 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Common control 9                      */
        sccb_dat[0] = 0x2c ; sccb_dat[1] = 0x0c ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x33 ; sccb_dat[1] = 0x78 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x3a ; sccb_dat[1] = 0x33 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x3b ; sccb_dat[1] = 0xfB ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x3e ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x43 ; sccb_dat[1] = 0x11 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x16 ; sccb_dat[1] = 0x10 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x4a ; sccb_dat[1] = 0x81 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x21 ; sccb_dat[1] = 0x99 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x24 ; sccb_dat[1] = 0x40 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Luminance signal High range           */
        sccb_dat[0] = 0x25 ; sccb_dat[1] = 0x38 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Luminance signal low range            */
        sccb_dat[0] = 0x26 ; sccb_dat[1] = 0x82 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /*                                       */
        sccb_dat[0] = 0x5c ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x63 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x46 ; sccb_dat[1] = 0x3f ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Frame length adjustment               */
        sccb_dat[0] = 0x0c ; sccb_dat[1] = 0x3c ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Common control 3                      */
        sccb_dat[0] = 0x61 ; sccb_dat[1] = 0x70 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Histogram algo low level              */
        sccb_dat[0] = 0x62 ; sccb_dat[1] = 0x80 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Histogram algo high level             */
        sccb_dat[0] = 0x7c ; sccb_dat[1] = 0x05 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x20 ; sccb_dat[1] = 0x80 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x28 ; sccb_dat[1] = 0x30 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x6c ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x6d ; sccb_dat[1] = 0x80 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x6e ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x70 ; sccb_dat[1] = 0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x71 ; sccb_dat[1] = 0x94 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x73 ; sccb_dat[1] = 0xc1 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x3d ; sccb_dat[1] = 0x34 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x5a ; sccb_dat[1] = 0x57 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Reserved                              */
        sccb_dat[0] = 0x12 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Common control 7                      */
        sccb_dat[0] = 0x11 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Clock Rate Control                   2*/
        sccb_dat[0] = 0x17 ; sccb_dat[1] = 0x11 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Horiz window start MSB 8bits          */
        sccb_dat[0] = 0x18 ; sccb_dat[1] = 0x75 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Horiz window end MSB 8bits            */
        sccb_dat[0] = 0x19 ; sccb_dat[1] = 0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Vert window line start MSB 8bits      */
        sccb_dat[0] = 0x1a ; sccb_dat[1] = 0x97 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ;   /* Vert window line end MSB 8bits        */
        sccb_dat[0] = 0x32 ; sccb_dat[1] = 0x36 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x03 ; sccb_dat[1] = 0x0f ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x37 ; sccb_dat[1] = 0x40 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x4f ; sccb_dat[1] = 0xbb ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x50 ; sccb_dat[1] = 0x9c ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x5a ; sccb_dat[1] = 0x57 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x6d ; sccb_dat[1] = 0x80 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x6d ; sccb_dat[1] = 0x38 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x39 ; sccb_dat[1] = 0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x35 ; sccb_dat[1] = 0x88 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x22 ; sccb_dat[1] = 0x0a ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x37 ; sccb_dat[1] = 0x40 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x23 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x34 ; sccb_dat[1] = 0xa0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x36 ; sccb_dat[1] = 0x1a ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x06 ; sccb_dat[1] = 0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x07 ; sccb_dat[1] = 0xc0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x0d ; sccb_dat[1] = 0xb7 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x0e ; sccb_dat[1] = 0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x4c ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xff ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xe5 ; sccb_dat[1] = 0x7f ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xf9 ; sccb_dat[1] = 0xc0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x41 ; sccb_dat[1] = 0x24 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xe0 ; sccb_dat[1] = 0x14 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x76 ; sccb_dat[1] = 0xff ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x33 ; sccb_dat[1] = 0xa0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x42 ; sccb_dat[1] = 0x20 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x43 ; sccb_dat[1] = 0x18 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x4c ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x87 ; sccb_dat[1] = 0xd0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x88 ; sccb_dat[1] = 0x3f ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xd7 ; sccb_dat[1] = 0x03 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xd9 ; sccb_dat[1] = 0x10 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xd3 ; sccb_dat[1] = 0x82 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xc8 ; sccb_dat[1] = 0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xc9 ; sccb_dat[1] = 0x80 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x7d ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x7c ; sccb_dat[1] = 0x03 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x7d ; sccb_dat[1] = 0x48 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x7c ; sccb_dat[1] = 0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x7d ; sccb_dat[1] = 0x20 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x7d ; sccb_dat[1] = 0x10 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x7d ; sccb_dat[1] = 0x0e ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x90 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0x0e ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0x1a ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0x31 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0x5a ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0x69 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0x75 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0x7e ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0x88 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0x8f ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0x96 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0xa3 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0xaf ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0xc4 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0xd7 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0xe8 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x91 ; sccb_dat[1] = 0x20 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x92 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0x06 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0xe3 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0x04 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0x03 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x93 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x96 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x97 ; sccb_dat[1] = 0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x97 ; sccb_dat[1] = 0x19 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x97 ; sccb_dat[1] = 0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x97 ; sccb_dat[1] = 0x0c ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x97 ; sccb_dat[1] = 0x24 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x97 ; sccb_dat[1] = 0x30 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x97 ; sccb_dat[1] = 0x28 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x97 ; sccb_dat[1] = 0x26 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x97 ; sccb_dat[1] = 0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x97 ; sccb_dat[1] = 0x98 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x97 ; sccb_dat[1] = 0x80 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x97 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x97 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xc3 ; sccb_dat[1] = 0xef ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xff ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xba ; sccb_dat[1] = 0xdc ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xbb ; sccb_dat[1] = 0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xb6 ; sccb_dat[1] = 0x24 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xb8 ; sccb_dat[1] = 0x33 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xb7 ; sccb_dat[1] = 0x20 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xb9 ; sccb_dat[1] = 0x30 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xb3 ; sccb_dat[1] = 0xb4 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xb4 ; sccb_dat[1] = 0xca ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xb5 ; sccb_dat[1] = 0x43 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xb0 ; sccb_dat[1] = 0x5c ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xb1 ; sccb_dat[1] = 0x4f ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xb2 ; sccb_dat[1] = 0x06 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xc7 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xc6 ; sccb_dat[1] = 0x51 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xc5 ; sccb_dat[1] = 0x11 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xc4 ; sccb_dat[1] = 0x9c ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xbf ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xbc ; sccb_dat[1] = 0x64 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa6 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x1e ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x6b ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x47 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x33 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x23 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x2e ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x85 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x42 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x33 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x23 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x1b ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x74 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x42 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x33 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xa7 ; sccb_dat[1] = 0x23 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xc0 ; sccb_dat[1] = 0xc8 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xc1 ; sccb_dat[1] = 0x96 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x8c ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x86 ; sccb_dat[1] = 0x3d ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x50 ; sccb_dat[1] = 0x92 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x51 ; sccb_dat[1] = 0x90 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x52 ; sccb_dat[1] = 0x2c ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x53 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x54 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x55 ; sccb_dat[1] = 0x88 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x5a ; sccb_dat[1] = 0x50 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x5b ; sccb_dat[1] = 0x3c ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x5c ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xd3 ; sccb_dat[1] = 0x04 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x7f ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xDA ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xe5 ; sccb_dat[1] = 0x1f ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xe1 ; sccb_dat[1] = 0x67 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xe0 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xdd ; sccb_dat[1] = 0x7f ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x05 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xff ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xe0 ; sccb_dat[1] = 0x04 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xc0 ; sccb_dat[1] = 0xc8 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xc1 ; sccb_dat[1] = 0x96 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x86 ; sccb_dat[1] = 0x3d ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x50 ; sccb_dat[1] = 0x92 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x51 ; sccb_dat[1] = 0x90 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x52 ; sccb_dat[1] = 0x2c ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x53 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x54 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x55 ; sccb_dat[1] = 0x88 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x57 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x5a ; sccb_dat[1] = 0x50 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x5b ; sccb_dat[1] = 0x3c ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x5c ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xd3 ; sccb_dat[1] = 0x04 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xe0 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xFF ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x05 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xDA ; sccb_dat[1] = 0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xDA ; sccb_dat[1] = 0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x98 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x99 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x00 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xff ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xe0 ; sccb_dat[1] = 0x04 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xc0 ; sccb_dat[1] = 0xc8 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xc1 ; sccb_dat[1] = 0x96 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x86 ; sccb_dat[1] = 0x3d ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x50 ; sccb_dat[1] = 0x89 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x51 ; sccb_dat[1] = 0x90 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x52 ; sccb_dat[1] = 0x2c ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x53 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x54 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x55 ; sccb_dat[1] = 0x88 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x57 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x5a ; sccb_dat[1] = 0xA0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x5b ; sccb_dat[1] = 0x78 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0x5c ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xd3 ; sccb_dat[1] = 0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        sccb_dat[0] = 0xe0 ; sccb_dat[1] = 0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 2) ; 
        break;

    case DEV_OV5642:
        CAM_ADDR=(0x78>>1);    
        // PLL
        sccb_dat[0]=0x31 ; sccb_dat[1]=0x03; sccb_dat[2]=0x93; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // Reset system
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x08; sccb_dat[2]=0x82; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // output enable(1)
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x17; sccb_dat[2]=0x7f; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // output enable(2)
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x18; sccb_dat[2]=0xfc; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // HV offset setting
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x10 ; sccb_dat[2]=0xc2; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // analog setting
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x15 ; sccb_dat[2]=0xf0; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // block init
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x00 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x01 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x02 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x03 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x00 ; sccb_dat[2]=0xf8 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x01 ; sccb_dat[2]=0x48 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x02 ; sccb_dat[2]=0x5c ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x03 ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // block clock enable
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x04 ; sccb_dat[2]=0x07 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x05 ; sccb_dat[2]=0xb7 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x06 ; sccb_dat[2]=0x43 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x07 ; sccb_dat[2]=0x37 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // PLL(FPS)
        // 0x3011=0x08:15fps
        // 0x3011=0x10:30fps
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x11 ; sccb_dat[2]=0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x10 ; sccb_dat[2]=0x10 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ; 
        // VFIFO
        sccb_dat[0]=0x46 ; sccb_dat[1]=0x0c ; sccb_dat[2]=0x22 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // unknown settings
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x15 ; sccb_dat[2]=0x04 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // array control
        sccb_dat[0]=0x37 ; sccb_dat[1]=0x0d ; sccb_dat[2]=0x06 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // analog settings
        sccb_dat[0]=0x37 ; sccb_dat[1]=0x0c ; sccb_dat[2]=0xa0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x02 ; sccb_dat[2]=0xfc ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x12 ; sccb_dat[2]=0xff ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x34 ; sccb_dat[2]=0xc0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x13 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x05 ; sccb_dat[2]=0x7c ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // array control 
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x21 ; sccb_dat[2]=0x09 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // analog settings
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x22 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x04 ; sccb_dat[2]=0x40 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x03 ; sccb_dat[2]=0xa7 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x03 ; sccb_dat[2]=0x27 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // black color level 
        sccb_dat[0]=0x40 ; sccb_dat[1]=0x00 ; sccb_dat[2]=0x21 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x40 ; sccb_dat[1]=0x1d ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // analog settings
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x00 ; sccb_dat[2]=0x54 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x05 ; sccb_dat[2]=0x04 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x06 ; sccb_dat[2]=0x3f ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // flicker
        sccb_dat[0]=0x3c ; sccb_dat[1]=0x01 ; sccb_dat[2]=0x80 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // ISP
        sccb_dat[0]=0x50 ; sccb_dat[1]=0x00 ; sccb_dat[2]=0x4f ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // unknown 
        sccb_dat[0]=0x50 ; sccb_dat[1]=0x20 ; sccb_dat[2]=0x04 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // AWB
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x81 ; sccb_dat[2]=0x79 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x82 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x85 ; sccb_dat[2]=0x22 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x97 ; sccb_dat[2]=0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // ISP
        sccb_dat[0]=0x50 ; sccb_dat[1]=0x01 ; sccb_dat[2]=0xff ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // UV adjust
        sccb_dat[0]=0x55 ; sccb_dat[1]=0x00 ; sccb_dat[2]=0x0a ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x55 ; sccb_dat[1]=0x04 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x55 ; sccb_dat[1]=0x05 ; sccb_dat[2]=0x7f ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // ISP
        sccb_dat[0]=0x50 ; sccb_dat[1]=0x80 ; sccb_dat[2]=0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // MIPI
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x0e ; sccb_dat[2]=0x18 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // unknown 
        sccb_dat[0]=0x46 ; sccb_dat[1]=0x10 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // DVP output
        sccb_dat[0]=0x47 ; sccb_dat[1]=0x1d ; sccb_dat[2]=0x05 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x47 ; sccb_dat[1]=0x08 ; sccb_dat[2]=0x06 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // analog setting register 
        sccb_dat[0]=0x37 ; sccb_dat[1]=0x10 ; sccb_dat[2]=0x10 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x32 ; sccb_dat[2]=0x41 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x37 ; sccb_dat[1]=0x02 ; sccb_dat[2]=0x40 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x20 ; sccb_dat[2]=0x37 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x31 ; sccb_dat[2]=0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // output setting
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x08 ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x09 ; sccb_dat[2]=0x80 ;// H-size:0x0280 = 640
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x0a ; sccb_dat[2]=0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x0b ; sccb_dat[2]=0xe0 ;// V-size:0x01e0 = 480
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x0e ; sccb_dat[2]=0x07 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x0f ; sccb_dat[2]=0xd0 ;// V-pixel:0x07d0 = 2000
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // select output format
        sccb_dat[0]=0x50 ; sccb_dat[1]=0x1f ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // ISP Settings
        sccb_dat[0]=0x50 ; sccb_dat[1]=0x00 ; sccb_dat[2]=0x4f ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // output format settings
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x1e ; sccb_dat[2]=0x2a ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x50 ; sccb_dat[1]=0x02 ; sccb_dat[2]=0xf8 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x50 ; sccb_dat[1]=0x1f ; sccb_dat[2]=0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x43 ; sccb_dat[1]=0x00 ; sccb_dat[2]=0x61 ;// RGB565
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // AEC Settings         
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x03 ; sccb_dat[2]=0x07 ;// VTS Manual 
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x01 ; sccb_dat[2]=0x73 ;// shutter speed
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x02 ; sccb_dat[2]=0x80 ;// shutter speed
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x0b ; sccb_dat[2]=0x00 ;// AGC Gain
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x03 ; sccb_dat[2]=0x07 ;// VTS manual  
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // unknown
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x24 ; sccb_dat[2]=0x11 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // AEC Settings
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x01 ; sccb_dat[2]=0x1e ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x02 ; sccb_dat[2]=0x80 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // AGC Settings 
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x0b ; sccb_dat[2]=0x7f ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // output timing settings
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x0c ; sccb_dat[2]=0x0c ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x0d ; sccb_dat[2]=0x80 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x0e ; sccb_dat[2]=0x03 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x0f ; sccb_dat[2]=0xe8 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // flicker-less settings
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x0d ; sccb_dat[2]=0x04 ;// 60Hz
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x0e ; sccb_dat[2]=0x03 ;// 50Hz
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // timing and mirror and flip settings
        // ov5642 default position (0 degrees)
        // 0x3818 = 0xC1;
        // 0x3621 = 0xC7;
        // or if you use ov5642 upside down (turn 180 degrees)
        // 0x3818 = 0xA1;
        // 0x3621 = 0xA7;
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x18 ; sccb_dat[2]=0xA1 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // analog settings register 
        sccb_dat[0]=0x37 ; sccb_dat[1]=0x05 ; sccb_dat[2]=0xdb ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x37 ; sccb_dat[1]=0x0a ; sccb_dat[2]=0x81 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // array control 
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x21 ; sccb_dat[2]=0xA7; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // output timing
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x01 ; sccb_dat[2]=0x50 ;// H-Start:80
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x03 ; sccb_dat[2]=0x08 ;// V-Start:8
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // unknown
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x27 ; sccb_dat[2]=0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // HV offset settings
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x10 ; sccb_dat[2]=0xc0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // output timing
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x04 ; sccb_dat[2]=0x05 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x05 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // Statistics Settings
        sccb_dat[0]=0x56 ; sccb_dat[1]=0x82 ; sccb_dat[2]=0x05 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x56 ; sccb_dat[1]=0x83 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // output timing
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x06 ; sccb_dat[2]=0x03 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x07 ; sccb_dat[2]=0xc0 ;// V-pixel:960
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // Statistics Settings
        sccb_dat[0]=0x56 ; sccb_dat[1]=0x86 ; sccb_dat[2]=0x03 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x56 ; sccb_dat[1]=0x87 ; sccb_dat[2]=0xc0 ;// V-pixel:960
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #102:AEC Settings
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x00 ; sccb_dat[2]=0x78 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x1a ; sccb_dat[2]=0x04 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x13 ; sccb_dat[2]=0x30 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x18 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x19 ; sccb_dat[2]=0x7c ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #107: flicker-less settings
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x08 ; sccb_dat[2]=0x12 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x09 ; sccb_dat[2]=0xc0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x0a ; sccb_dat[2]=0x0f ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x0b ; sccb_dat[2]=0xa0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #111: block clock enable
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x04 ; sccb_dat[2]=0xff ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #112: AEC Settings
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x0c ; sccb_dat[2]=0x07 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x0d ; sccb_dat[2]=0xd0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x00 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x01 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x02 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #117: AGC/AEC Settings
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x0a ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x0b ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x35 ; sccb_dat[1]=0x03 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #120: De-Noise Settings
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x8a ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x8b ; sccb_dat[2]=0x04 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x8c ; sccb_dat[2]=0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x8d ; sccb_dat[2]=0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x8e ; sccb_dat[2]=0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x8f ; sccb_dat[2]=0x10 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x90 ; sccb_dat[2]=0x10 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x92 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x93 ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x94 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x95 ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x96 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x97 ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x98 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x99 ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x9a ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x9b ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x9c ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x9d ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x9e ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x9f ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #141: AEC Settings
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x0f ; sccb_dat[2]=0x3c ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x10 ; sccb_dat[2]=0x30 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x1b ; sccb_dat[2]=0x3c ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x1e ; sccb_dat[2]=0x30 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x11 ; sccb_dat[2]=0x70 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x1f ; sccb_dat[2]=0x10 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #147: system settings 
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x30 ; sccb_dat[2]=0x0b ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #148: AEC Settings
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x02 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x03 ; sccb_dat[2]=0x7d ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x04 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x14 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x15 ; sccb_dat[2]=0x7d ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x16 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x00 ; sccb_dat[2]=0x7c ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #155: flicker-less settings
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x08 ; sccb_dat[2]=0x09 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x09 ; sccb_dat[2]=0x60 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x0a ; sccb_dat[2]=0x07 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x0b ; sccb_dat[2]=0xd0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x0d ; sccb_dat[2]=0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x0e ; sccb_dat[2]=0x06 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #161: AWB settings
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x93 ; sccb_dat[2]=0x70 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #162: analog settings
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x20 ; sccb_dat[2]=0x57 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x37 ; sccb_dat[1]=0x03 ; sccb_dat[2]=0x98 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x37 ; sccb_dat[1]=0x04 ; sccb_dat[2]=0x1c ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #165: unknown settings
        sccb_dat[0]=0x58 ; sccb_dat[1]=0x9b ; sccb_dat[2]=0x04 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x58 ; sccb_dat[1]=0x9a ; sccb_dat[2]=0xc5 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #167: De-Noise Settings
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x8a ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x8b ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x8c ; sccb_dat[2]=0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x8d ; sccb_dat[2]=0x10 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x8e ; sccb_dat[2]=0x20 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x8f ; sccb_dat[2]=0x28 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x90 ; sccb_dat[2]=0x30 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x92 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x93 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x94 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x95 ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x96 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x97 ; sccb_dat[2]=0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x98 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x99 ; sccb_dat[2]=0x10 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x9a ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x9b ; sccb_dat[2]=0x20 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x9c ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x9d ; sccb_dat[2]=0x28 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x9e ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x9f ; sccb_dat[2]=0x30 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x52 ; sccb_dat[1]=0x82 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #189: CIP
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x00 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x01 ; sccb_dat[2]=0x20 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x02 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x03 ; sccb_dat[2]=0x7c ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x0c ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x0d ; sccb_dat[2]=0x0c ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x0e ; sccb_dat[2]=0x20 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x0f ; sccb_dat[2]=0x80 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x10 ; sccb_dat[2]=0x20 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x11 ; sccb_dat[2]=0x80 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x08 ; sccb_dat[2]=0x20 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x09 ; sccb_dat[2]=0x40 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x04 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x05 ; sccb_dat[2]=0x30 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x06 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x07 ; sccb_dat[2]=0x80 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x14 ; sccb_dat[2]=0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x15 ; sccb_dat[2]=0x20 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x19 ; sccb_dat[2]=0x30 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x16 ; sccb_dat[2]=0x10 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x17 ; sccb_dat[2]=0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x18 ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #211: Color Matrix Settings
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x80 ; sccb_dat[2]=0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x81 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x82 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x83 ; sccb_dat[2]=0x4e ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x84 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x85 ; sccb_dat[2]=0x0f ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x86 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x87 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x88 ; sccb_dat[2]=0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x89 ; sccb_dat[2]=0x15 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x8a ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x8b ; sccb_dat[2]=0x31 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x8c ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x8d ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x8e ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x8f ; sccb_dat[2]=0x0f ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x90 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x91 ; sccb_dat[2]=0xab ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x92 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x93 ; sccb_dat[2]=0xa2 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x53 ; sccb_dat[1]=0x94 ; sccb_dat[2]=0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #232: Gamma Setings 
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x80 ; sccb_dat[2]=0x14 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x81 ; sccb_dat[2]=0x21 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x82 ; sccb_dat[2]=0x36 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x83 ; sccb_dat[2]=0x57 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x84 ; sccb_dat[2]=0x65 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x85 ; sccb_dat[2]=0x71 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x86 ; sccb_dat[2]=0x7d ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x87 ; sccb_dat[2]=0x87 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x88 ; sccb_dat[2]=0x91 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x89 ; sccb_dat[2]=0x9a ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x8a ; sccb_dat[2]=0xaa ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x8b ; sccb_dat[2]=0xb8 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x8c ; sccb_dat[2]=0xcd ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x8d ; sccb_dat[2]=0xdd ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x8e ; sccb_dat[2]=0xea ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x8f ; sccb_dat[2]=0x10 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x90 ; sccb_dat[2]=0x05 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x91 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x92 ; sccb_dat[2]=0x04 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x93 ; sccb_dat[2]=0x20 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x94 ; sccb_dat[2]=0x03 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x95 ; sccb_dat[2]=0x60 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x96 ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x97 ; sccb_dat[2]=0xb8 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x98 ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x99 ; sccb_dat[2]=0x86 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x9a ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x9b ; sccb_dat[2]=0x5b ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x9c ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x9d ; sccb_dat[2]=0x3b ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x9e ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0x9f ; sccb_dat[2]=0x1c ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xa0 ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xa1 ; sccb_dat[2]=0x04 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xa2 ; sccb_dat[2]=0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xa3 ; sccb_dat[2]=0xed ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xa4 ; sccb_dat[2]=0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xa5 ; sccb_dat[2]=0xc5 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xa6 ; sccb_dat[2]=0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xa7 ; sccb_dat[2]=0xa5 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xa8 ; sccb_dat[2]=0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xa9 ; sccb_dat[2]=0x6c ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xaa ; sccb_dat[2]=0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xab ; sccb_dat[2]=0x41 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xac ; sccb_dat[2]=0x01 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xad ; sccb_dat[2]=0x20 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xae ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x54 ; sccb_dat[1]=0xaf ; sccb_dat[2]=0x16 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #280: AWB Settings 
        sccb_dat[0]=0x34 ; sccb_dat[1]=0x06 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x92 ; sccb_dat[2]=0x04 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x91 ; sccb_dat[2]=0xf8 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x93 ; sccb_dat[2]=0xf0 ;//R
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x94 ; sccb_dat[2]=0x40 ;//G
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x95 ; sccb_dat[2]=0xf0 ;//B
        reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x8d ; sccb_dat[2]=0x3d ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x8f ; sccb_dat[2]=0x54 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x8e ; sccb_dat[2]=0x3d ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x90 ; sccb_dat[2]=0x54 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x8b ; sccb_dat[2]=0xc0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x8c ; sccb_dat[2]=0xbd ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x87 ; sccb_dat[2]=0x18 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x88 ; sccb_dat[2]=0x18 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x89 ; sccb_dat[2]=0x6e ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x8a ; sccb_dat[2]=0x68 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x86 ; sccb_dat[2]=0x1c ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x81 ; sccb_dat[2]=0x50 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #298: AWB Settings
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x84 ; sccb_dat[2]=0x25 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x82 ; sccb_dat[2]=0x11 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x83 ; sccb_dat[2]=0x14 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x84 ; sccb_dat[2]=0x25 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x51 ; sccb_dat[1]=0x85 ; sccb_dat[2]=0x24 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // ISP Settings
        sccb_dat[0]=0x50 ; sccb_dat[1]=0x25 ; sccb_dat[2]=0x82 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #304: AEC Settings
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x0f ; sccb_dat[2]=0x7e ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x10 ; sccb_dat[2]=0x72 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x1b ; sccb_dat[2]=0x80 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x1e ; sccb_dat[2]=0x70 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x11 ; sccb_dat[2]=0xd0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x1f ; sccb_dat[2]=0x40 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #310: Digital effect 
        sccb_dat[0]=0x55 ; sccb_dat[1]=0x83 ; sccb_dat[2]=0x40 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x55 ; sccb_dat[1]=0x84 ; sccb_dat[2]=0x40 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x55 ; sccb_dat[1]=0x80 ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #313: Analog settings register
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x33 ; sccb_dat[2]=0x07 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x37 ; sccb_dat[1]=0x02 ; sccb_dat[2]=0x10 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x37 ; sccb_dat[1]=0x03 ; sccb_dat[2]=0xb2 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x37 ; sccb_dat[1]=0x04 ; sccb_dat[2]=0x18 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x37 ; sccb_dat[1]=0x0b ; sccb_dat[2]=0x40 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // array control 
        sccb_dat[0]=0x37 ; sccb_dat[1]=0x0d ; sccb_dat[2]=0x02 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        // #319: analog settings register 
        sccb_dat[0]=0x36 ; sccb_dat[1]=0x20 ; sccb_dat[2]=0x52 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        #if 0 // pclk=6MHz
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x11 ; sccb_dat[2]=0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x12 ; sccb_dat[2]=0x00 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x30 ; sccb_dat[1]=0x10 ; sccb_dat[2]=0x70 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x46 ; sccb_dat[1]=0x0c ; sccb_dat[2]=0x22 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x0c ; sccb_dat[2]=0x0c ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x38 ; sccb_dat[1]=0x0d ; sccb_dat[2]=0x80 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x00 ; sccb_dat[2]=0x78 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x08 ; sccb_dat[2]=0x09 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x09 ; sccb_dat[2]=0x60 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x0a ; sccb_dat[2]=0x07 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x0b ; sccb_dat[2]=0xd0 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x0d ; sccb_dat[2]=0x08 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        sccb_dat[0]=0x3a ; sccb_dat[1]=0x0e ; sccb_dat[2]=0x06 ; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        #endif
        //VSYNC Active-Low & Gate PCLK under VSYNC & HREF
        sccb_dat[0]=0x47 ; sccb_dat[1]=0x40 ; sccb_dat[2]=0x2d; reg_write(i2c, CAM_ADDR, sccb_dat, 3) ;
        break;
    default:
        break;
    }
}

// Write 1 byte to the specified register
int32_t reg_write(  i2c_inst_t *i2c, 
                const uint32_t addr, 
                uint8_t *buf,
                const uint8_t nbytes) {

    int32_t num_bytes_read = 0;
    uint8_t msg[nbytes];

    // Check to make sure caller is sending 1 or more bytes
    if (nbytes < 1) {
        return 0;
    }

    // Append register address to front of data packet
    for (int32_t i = 0; i < nbytes; i++) {
        msg[i] = buf[i];
    }

    // Write data to register(s) over I2C
    i2c_write_blocking(i2c, addr, msg, nbytes, false);

    return num_bytes_read;
}

// Read byte(s) from specified register. If nbytes > 1, read from consecutive
// registers.
int32_t reg_read(  i2c_inst_t *i2c,
                const uint32_t addr,
                uint8_t *buf,
                const uint8_t nbytes) {

    int32_t num_bytes_read = 0;

    // Check to make sure caller is asking for 1 or more bytes
    if (nbytes < 1) {
        return 0;
    }

    // Read data from register(s) over I2C
    num_bytes_read = i2c_read_blocking(i2c, addr, buf, nbytes, false);

    return num_bytes_read;
}


