#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "picampinos.pio.h"
#include "iot_sram.h"
//#include "class/cdc/cdc_device.h" // for uart(binary output)
#include "hardware/pwm.h"
#include "hardware/irq.h"
#include "hardware/dma.h"
#include "pico/multicore.h"

#include "cam.h"

#if USE_EZSPI_SLAVE
#include "ezspi_slave.h"
#endif
#if USE_100BASE_FX
#include "sfp_hw.h"
#endif


volatile bool is_captured = false; 

// priority: sram read > sram write
volatile bool ram_ind_read  = false;    // indicate Read
volatile bool ram_in_write  = false;    // now writing image 

volatile bool irq_indicate_reset = true;

volatile int32_t psram_access = 0; // write buffer:+=1, read buffer:-=1
// init PIO
PIO pio_cam = pio1;
PIO pio_iot = pio0;

#if USE_EZSPI_SLAVE
PIO pio_spi = pio1; // same PIO with pio_iot
#endif
#if USE_100BASE_FX
PIO pio_sfp = pio0; // same PIO with pio_iot
#endif
// statemachine's pointer
uint32_t sm_cam;    // CAMERA's state machines

// dma channels
uint32_t DMA_CAM_RD_CH0 ;
uint32_t DMA_CAM_RD_CH1 ;

// private functions and buffers
uint32_t* cam_ptr;  // pointer of camera buffer
uint32_t* cam_ptr2; // back half pointer of cam_ptr.
uint32_t* iot_ptr;  // pointer of IoT RAM's read buffer.

dma_channel_config get_cam_config(PIO pio, uint32_t sm, uint32_t dma_chan);
void set_pwm_freq_kHz(uint32_t freq_khz, uint32_t system_clk_khz, uint8_t gpio_num);
void cam_handler();
void printbuf(uint8_t buf[], size_t len) ;

void init_cam(uint8_t DEVICE_IS) {

    // Initialize CAMERA
    set_pwm_freq_kHz(24000, SYS_CLK_KHZ, PIN_PWM0); // XCLK 24MHz -> OV5642,OV2640
    sleep_ms(1000);

    sccb_init(DEVICE_IS, I2C1_SDA, I2C1_SCL); // sda,scl=(gp26,gp27). see 'sccb_if.c' and 'cam.h'
    sleep_ms(3000);
    
    uint32_t offset_cam = pio_add_program(pio_cam, &picampinos_program);
    uint32_t sm = pio_claim_unused_sm(pio_cam, true);
    //printf("camera:pio=%d, sm = %d, offset=%d\r\n",(uint32_t)pio_cam, sm, offset_cam);
    picampinos_program_init(pio_cam, sm_cam, offset_cam, CAM_BASE_PIN, 11);// VSYNC,HREF,PCLK,D[2:9] : total 11 pins
    pio_sm_set_enabled(pio_cam, sm_cam, false);
    pio_sm_clear_fifos(pio_cam, sm_cam);
    pio_sm_restart(pio_cam, sm_cam);        
    pio_sm_set_enabled(pio_cam, sm_cam, true);
    
    // Initialize IoT SRAM
    iot_sram_init(pio_iot);

    // init DMA
    DMA_CAM_RD_CH0 = dma_claim_unused_channel(true);
    DMA_CAM_RD_CH1 = dma_claim_unused_channel(true);

    // buffer of camera data is 640 * 480 * 2 bytes (RGB565 = 16 bits = 2 bytes)
    // but rp2040 has no such a huge ram.
    // so, transfer to IoT-SRAM using lesser ram BLOCK.
    // camera buffer
    cam_ptr = (uint32_t *)calloc((CAM_BUF_SIZE / sizeof(uint32_t)), sizeof(uint32_t));
    cam_ptr2 = &cam_ptr[(CAM_BUF_HALF / sizeof(uint32_t))];
    iot_ptr = (uint32_t *)calloc((CAM_BUF_SIZE / sizeof(uint32_t)) + SFP_HEADER_WORDS, sizeof(uint32_t)); // same size of cam_ptr
}

void config_cam_buffer() {  
    // ------------------ CAMERA READ: withDMA   --------------------------------

    is_captured = false;
    // disable IRQ
    irq_set_enabled(DMA_IRQ_0, false);
    
    // (2) 1st DMA Channel Config
    dma_channel_config c;
    c = get_cam_config(pio_cam, sm_cam, DMA_CAM_RD_CH1);
    // trigger DMA_CAM_RD_CH0 when DMA_CAM_RD_CH1 completes. (ping-pong)
    channel_config_set_chain_to(&c,DMA_CAM_RD_CH0);   
    dma_channel_configure(DMA_CAM_RD_CH1, &c,
        cam_ptr2,                       // Destination pointer(back half of buffer)
        &pio_cam->rxf[sm_cam],          // Source pointer
        CAM_BUF_HALF/sizeof(uint32_t),  // Number of transfers
        false                           // Don't Start yet
    );

    // (1) 0th DMA Channel Config
    c = get_cam_config(pio_cam, sm_cam, DMA_CAM_RD_CH0);
    // trigger DMA_CAM_RD_CH1 when DMA_CAM_RD_CH0 completes.
    channel_config_set_chain_to(&c,DMA_CAM_RD_CH1); 
    dma_channel_configure(DMA_CAM_RD_CH0, &c,
        cam_ptr,                        // Destination pointer(front half of buffer)
        &pio_cam->rxf[sm_cam],          // Source pointer
        CAM_BUF_HALF/sizeof(uint32_t),  // Number of transfers
        false                           // Don't Start yet
    );

    // IRQ settings
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH1, true);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH0, true);
    irq_set_exclusive_handler(DMA_IRQ_0, cam_handler);
    //irq_indicate_reset = true;
    sem_init(&psram_sem, 1, 1); // init semaphore
    // enable IRQ    
    irq_set_enabled(DMA_IRQ_0, true);
}

void start_cam() {

    // Start DMA
    dma_channel_abort(DMA_CAM_RD_CH0);
    dma_start_channel_mask(1u << DMA_CAM_RD_CH0);

    // camera transfer settings(for video)
    pio_sm_put_blocking(pio_cam, sm_cam, 0);                                       // X=0 : reserved
    pio_sm_put_blocking(pio_cam, sm_cam, (CAM_FUL_SIZE/sizeof(uint32_t) - 1));     // Y: total words in an image 

    // wait until transfer finish
    // while(false == is_captured);    
}

void uartout_cam() {
    // read Image
    printf("!srt\r\n");
    sleep_ms(30);
    
    is_captured = false;
    while(!is_captured);    // wait until an image captured

    while(ram_in_write);    // wait until writing ram finished
    ram_ind_read = true;      // start to read
    
    int32_t iot_addr = 0;
    int32_t *b;
    b = iot_ptr;
    for (uint32_t h = 0 ; h < 480 ; h = h + BLOCK) {
        iot_sram_read(pio_iot, (uint32_t *)b, iot_addr, CAM_BUF_SIZE, DMA_IOT_RD_CH); //pio, sm, buffer, start_address, length 
        for (uint32_t i = 0 ; i < CAM_BUF_SIZE/sizeof(uint32_t) ; i++) {
            printf("0x%08X\r\n",b[i]);
        }
        // increment iot sram's address
        iot_addr = iot_addr + CAM_BUF_SIZE;

    }
    ram_ind_read = false;
}

/*
void uartout_bin_cam() {
    // read Image
    sleep_ms(30);
    is_captured = false;
    while(!is_captured);    // wait until an image captured
    
    while(ram_in_write);    // wait until writing ram finished
    ram_ind_read = true;    // indicate to read
    
    int32_t iot_addr = 0;
    int32_t *b;
    b = iot_ptr;
    for (uint32_t h = 0 ; h < 480 ; h = h + BLOCK) {
        iot_sram_read(pio_iot,(uint32_t *)b, iot_addr, CAM_BUF_SIZE, DMA_IOT_RD_CH); //pio, sm, buffer, start_address, length 
        for (uint32_t i = 0 ; i < CAM_BUF_SIZE/sizeof(uint32_t) ; i++) {
            //printf("0x%08X\r\n",b[i]);
            tud_cdc_write(&b[i], sizeof(uint32_t));
            tud_cdc_write_flush();
            sleep_us(150);
        }
        // increment iot sram's address
        iot_addr = iot_addr + CAM_BUF_SIZE;

    }
    sleep_ms(300);
    ram_ind_read = false;
}
*/

#if USE_EZSPI_SLAVE
void spiout_cam() {    
    uint8_t BUF_LEN = 10;
    uint8_t in_buf[BUF_LEN]; 
    uint8_t out_buf[BUF_LEN];
    
    is_captured = false;
    sleep_ms(80);
    
    
    while(!is_captured);    // wait until an image captured
    while(ram_in_write);    // wait until writing ram finished
    ram_ind_read = true;      // start to read
    
    init_spi_slave(pio_spi);

    int32_t iot_addr = 0;
    int32_t *b;
    uint32_t resp;
    b = iot_ptr;

    // send header
    write_word_spi_slave(pio_spi, 0xDEADBEEF);
    for (uint32_t h = 0 ; h < 480 ; h = h + BLOCK) {
        iot_sram_read(pio_iot, (uint32_t *)b, iot_addr, CAM_BUF_SIZE, DMA_IOT_RD_CH); //pio, sm, buffer, start_address, length         
        

        for (uint32_t i = 0 ; i < CAM_BUF_SIZE/sizeof(uint32_t) ; i += 640*2 /sizeof(uint32_t)) {
            write_blocking_spi_slave(pio_spi, &b[i], 640*2);
        }
        // increment iot sram's address
        iot_addr = iot_addr + CAM_BUF_SIZE;
    }
   
    //deinit_spi_slave();
    ram_ind_read = false;
}
#endif

#if USE_100BASE_FX
void sfp_cam() {    

    static int32_t iot_addr = 0;
    sfp_hw_init(pio_sfp);

    while(1) {

        
        int32_t *b;
        uint32_t resp;
        b = iot_ptr + SFP_HEADER_WORDS; // Header offset

        // send header
        // frame start: 
        // '0xdeadbeef' + row_size_in_words(unit is in words(not bytes)) + columb_size_in_words(total blocks per frame)
        uint32_t a[4] = { 0xdeadbeef, 480,640*2/sizeof(uint32_t), 480 };
        
        sfp_send(&a, sizeof(uint32_t)*4);
        
        for (uint32_t h = 0 ; h < 480 ; h = h + BLOCK/2) {
            
            while(psram_access < NUM_COMP_FRM / 2){};   // 50% of all data
            psram_access = psram_access - 1;            // 
            sem_acquire_blocking(&psram_sem);
            iot_sram_read(pio_iot, (uint32_t *)b, iot_addr, CAM_BUF_HALF, DMA_IOT_RD_CH); //pio, sm, buffer, start_address, length         
            sem_release(&psram_sem);

            for (uint32_t i = 0 ; i < CAM_BUF_HALF/sizeof(uint32_t) ; i+=320) {
                //printf("0x%08X\r\n",b[i]);

                sfp_send_with_header(0xbeefbeef,(h+i/320)+1,1,320, &(b[i]) - SFP_HEADER_WORDS, sizeof(uint32_t)*320);

                // for(uint32_t j = 0; j < 320;j++){
                //      printf("0x%08X\r\n",b[i+j]);
                // }

            }

            // increment iot sram's address
            iot_addr = iot_addr + CAM_BUF_HALF;
        
        }

        if(iot_addr > CAM_TOTAL_LEN - 1) {
            iot_addr = 0;
        }
        // send dummy data
        
        //for(uint32_t i = 0 ; i < 5 ; i++) {
            a[0] = 0xdeaddead ;
            sfp_send(&a, sizeof(uint32_t)*1);
        //}
    }
}
#endif

void free_cam() {
 
    // Disable IRQ settings
    irq_set_enabled(DMA_IRQ_0, false);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH1, false);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH0, false);
    dma_channel_abort(DMA_CAM_RD_CH0);
    dma_channel_abort(DMA_CAM_RD_CH0);
   
    free(cam_ptr);
    free(iot_ptr);
}

/// camera dma config
dma_channel_config get_cam_config(PIO pio, uint32_t sm, uint32_t dma_chan) {
    dma_channel_config c = dma_channel_get_default_config(dma_chan);    
    channel_config_set_read_increment(&c, false);
    channel_config_set_write_increment(&c, true);
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm, false));
    return c;
}

void cam_handler() {
    static uint32_t iot_addr = 0;
    static uint32_t num_of_call_this = 0;
    static uint32_t* b;
    uint32_t dma_chan;

    // first call?
    if(true == irq_indicate_reset) {
        num_of_call_this = 0;
        iot_addr = 0;
        is_captured = false;
        irq_indicate_reset = false;
    }

     // write iot sram
    if(0 == (num_of_call_this & 0x01)) {
        // even
        b = cam_ptr;
        dma_chan = DMA_CAM_RD_CH0; 
    } else{ 
        //odd
        b = cam_ptr2;
        dma_chan = DMA_CAM_RD_CH1;
    }


    psram_access = psram_access + 1;
    if(psram_access > NUM_COMP_FRM){psram_access = 0;}
    sem_acquire_blocking(&psram_sem);
    iot_sram_write(pio_iot, b, iot_addr, CAM_BUF_HALF, DMA_IOT_WR_CH); //pio, sm, buffer, start_address, length
    sem_release(&psram_sem);

    // increment iot sram's address
    iot_addr = iot_addr + CAM_BUF_HALF;

    // clear interrupt flag
    dma_hw->ints0 = 1u << dma_chan;
    
    // reset write address pointer
    dma_channel_set_write_addr(dma_chan, b, false);  
    
    if(num_of_call_this < NUM_COMP_FRM - 1) {
        num_of_call_this++;
    }
    else {
        num_of_call_this = 0;
        iot_addr = 0;
        //is_captured = true;
    }

    return;        
}

//// PWM
void set_pwm_freq_kHz(uint32_t freq_khz, uint32_t system_clk_khz, uint8_t gpio_num) {

    uint32_t pwm0_slice_num;
    uint32_t period;
    static pwm_config pwm0_slice_config;

    period = system_clk_khz / freq_khz - 1;
    if (period < 2) period = 2;

  
    gpio_set_function( gpio_num, GPIO_FUNC_PWM );
    pwm0_slice_num = pwm_gpio_to_slice_num( gpio_num );

    
    // config
    pwm0_slice_config = pwm_get_default_config();
    pwm_config_set_wrap( &pwm0_slice_config, period );
    
    // set clk div
    pwm_config_set_clkdiv( &pwm0_slice_config, 1 );

    // set PWM start
    pwm_init( pwm0_slice_num, &pwm0_slice_config, true );
    pwm_set_gpio_level( gpio_num, ( pwm0_slice_config.top * 0.50 ) ); // duty:50%    
}



