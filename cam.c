#include <stdio.h>
#include "picocam.pio.h"
#include "iot_sram.pio.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"
#include "hardware/dma.h"
#include "cam.h"

// camera buffer size
// 640x480, RGB565 picture needs 640x480x2 bytes of buffers.
// but RP2040 has no capacity such as huge buffers.
// so, using DMA and IoT SRAM and lesser buffer to store them.
#define BLOCK           (160)               // maximum: 160
#define CAM_FUL_SIZE    (640 * 480 * 2)
#define CAM_BUF_SIZE    (640 * BLOCK * 2)   // in bytes
#define CAM_BUF_HALF    (CAM_BUF_SIZE / 2)  // in bytes

static uint32_t* in_data;  // pointer of camera buffer
static uint32_t* in_data2; // back half pointer of in_data.

volatile static bool is_captured = false;

// init PIO
PIO pio_cam = pio0;
PIO pio_ram = pio1;

// statemachine's pointer
uint32_t sm_cam;    // CAMERA's state machines
uint32_t sm_ram;    // IoT SRAM's state machines

// dma channels
int32_t DMA_CAM_RD_CH0 ;
int32_t DMA_CAM_RD_CH1 ;
int32_t DMA_CAM_RD_CH2 ;
int32_t DMA_CAM_RD_CH3 ;
int32_t DMA_CAM_RD_CH4 ;
int32_t DMA_CAM_RD_CH5 ;
int32_t DMA_IOT_RD_CH  ;
int32_t DMA_IOT_WR_CH  ;




// private functions and buffers
dma_channel_config get_cam_config(PIO pio, uint32_t sm, uint32_t dma_chan);
void cam_handler();
void set_pwm_freq_kHz(uint32_t freq_khz, uint32_t system_clk_khz, uint8_t gpio_num);



//#if USE_CAMERA_SYSTEM
void init_cam(uint8_t DEVICE_IS){


    // Initialize CAMERA
    set_pwm_freq_kHz(24000, SYS_CLK_KHZ, PIN_PWM0); // XCLK 24MHz -> OV5642,OV2640
    sleep_ms(1000);

    sccb_init(DEVICE_IS); // sda,scl=(gp12,gp13). see 'sccb_if.c'
    sleep_ms(3000);
    
    uint32_t offset = pio_add_program(pio_cam, &picocam_program);
    uint32_t sm = pio_claim_unused_sm(pio_cam, true);
    picocam_program_init(pio_cam, sm_cam, offset, CAM_BASE_PIN, 11);// VSYNC,HREF,PCLK,D[2:9] : total 11 pins
    pio_sm_set_enabled(pio_cam, sm_cam, false);
    pio_sm_clear_fifos(pio_cam, sm_cam);
    pio_sm_restart(pio_cam, sm_cam);        
    pio_sm_set_enabled(pio_cam, sm_cam, true);

    // Initialize IoT SRAM
    uint32_t offset01 = pio_add_program(pio_ram, &iot_sram_program);
    uint32_t sm_ram = pio_claim_unused_sm(pio_ram, true);
    iot_sram_program_init(pio_ram, sm_ram, offset01, IOT_DAT_BASE_PIN, 4, IOT_SIG_BASE_PIN, 2); // : total 6 pins
    iot_sram_init(pio_ram, sm_ram);

    // init DMA
    DMA_CAM_RD_CH0 = dma_claim_unused_channel(true);
    DMA_CAM_RD_CH1 = dma_claim_unused_channel(true);
    DMA_CAM_RD_CH2 = dma_claim_unused_channel(true);
    DMA_CAM_RD_CH3 = dma_claim_unused_channel(true);
    DMA_CAM_RD_CH4 = dma_claim_unused_channel(true);
    DMA_CAM_RD_CH5 = dma_claim_unused_channel(true);
    DMA_IOT_RD_CH  = dma_claim_unused_channel(true);
    DMA_IOT_WR_CH  = dma_claim_unused_channel(true);

    // buffer of camera data is 640 * 480 * 2 bytes (RGB565 = 16 bits = 2 bytes)
    // but rp2040 has no such a huge ram.
    // so, transfer to IoT-SRAM using lesser ram BLOCK.
    // camera buffer
    in_data = (uint32_t *)calloc((CAM_BUF_SIZE / sizeof(uint32_t)), sizeof(uint32_t));
    in_data2 = &in_data[(CAM_BUF_HALF / sizeof(uint32_t))];
}
//#endif

void config_cam_buffer() {  
    // ------------------ CAMERA READ: withDMA   --------------------------------

    is_captured = false;
    irq_set_enabled(DMA_IRQ_0, false);
    // Capture Image
    // (5) 5th DMA Channel Config
    dma_channel_config c;
    c = get_cam_config(pio_cam, sm_cam, DMA_CAM_RD_CH5);

    dma_channel_configure(DMA_CAM_RD_CH5, &c,
        in_data2,                       // Destination pointer(back half of buffer)
        &pio_cam->rxf[sm_cam],          // Source pointer
        CAM_BUF_HALF/sizeof(uint32_t),  // Number of transfers
        false                           // Don't Start yet
    );
       
    // (4) 4th DMA Channel Config
    c = get_cam_config(pio_cam, sm_cam, DMA_CAM_RD_CH4);
    channel_config_set_chain_to(&c,DMA_CAM_RD_CH5); // trigger DMA_CAM_RD_CH5 when DMA_CAM_RD_CH4 completes.
    dma_channel_configure(DMA_CAM_RD_CH4, &c,
        in_data,                        // Destination pointer(front half of buffer)
        &pio_cam->rxf[sm_cam],          // Source pointer
        CAM_BUF_HALF/sizeof(uint32_t),  // Number of transfers
        false                           // Don't Start yet
    );

    // (3) 3rd DMA Channel Config
    c = get_cam_config(pio_cam, sm_cam, DMA_CAM_RD_CH3);
    channel_config_set_chain_to(&c,DMA_CAM_RD_CH4); // trigger DMA_CAM_RD_CH4 when DMA_CAM_RD_CH3 completes.    
    dma_channel_configure(DMA_CAM_RD_CH3, &c,
        in_data2,                       // Destination pointer(back half of buffer)
        &pio_cam->rxf[sm_cam],          // Source pointer
        CAM_BUF_HALF/sizeof(uint32_t),  // Number of transfers
        false                           // Don't Start yet
    );

    // (2) 2nd DMA Channel Config
    c = get_cam_config(pio_cam, sm_cam, DMA_CAM_RD_CH2);
    channel_config_set_chain_to(&c,DMA_CAM_RD_CH3); // trigger DMA_CAM_RD_CH3 when DMA_CAM_RD_CH2 completes.
    dma_channel_configure(DMA_CAM_RD_CH2, &c,
        in_data,                        // Destination pointer(front half of buffer)
        &pio_cam->rxf[sm_cam],          // Source pointer
        CAM_BUF_HALF/sizeof(uint32_t),  // Number of transfers
        false                           // Don't Start yet
    );

    // (1) 1st DMA Channel Config
    c = get_cam_config(pio_cam, sm_cam, DMA_CAM_RD_CH1);
    channel_config_set_chain_to(&c,DMA_CAM_RD_CH2); // trigger DMA_CAM_RD_CH2 when DMA_CAM_RD_CH1 completes.    
    dma_channel_configure(DMA_CAM_RD_CH1, &c,
        in_data2,                       // Destination pointer(back half of buffer)
        &pio_cam->rxf[sm_cam],          // Source pointer
        CAM_BUF_HALF/sizeof(uint32_t),  // Number of transfers
        false                           // Don't Start yet
    );

    // (0) 0th DMA Channel Config
    c = get_cam_config(pio_cam, sm_cam, DMA_CAM_RD_CH0);

    channel_config_set_chain_to(&c,DMA_CAM_RD_CH1); // trigger DMA_CAM_RD_CH1 when DMA_CAM_RD_CH0 completes.
    dma_channel_configure(DMA_CAM_RD_CH0, &c,
        in_data,                        // Destination pointer(front half of buffer)
        &pio_cam->rxf[sm_cam],          // Source pointer
        CAM_BUF_HALF/sizeof(uint32_t),  // Number of transfers
        false                           // Don't Start yet
    );

    // IRQ settings

    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH5, true);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH4, true);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH3, true);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH2, true);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH1, true);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH0, true);
    irq_set_exclusive_handler(DMA_IRQ_0, cam_handler);    
    irq_set_enabled(DMA_IRQ_0, true);
    
    
}

void capture_cam() {

// Start DMA
    dma_channel_abort(DMA_CAM_RD_CH0);
    dma_start_channel_mask(1u << DMA_CAM_RD_CH0);

    // camera transfer settings
    pio_sm_put_blocking(pio_cam, sm_cam, (CAM_FUL_SIZE - 1));   // X: total bytes 
    pio_sm_put_blocking(pio_cam, sm_cam, 0);                    // Y: Count Hsync 
    
    // wait until transfer finish
    while(false == is_captured);
    
}

void uartout_cam() {
    // read Image
    printf("!srt\r\n");

    int32_t iot_addr = 0;
    int32_t *b;
    b = in_data;
    for (uint32_t h = 0 ; h < 480 ; h = h + BLOCK) {
        iot_sram_read(pio_ram, sm_ram,(uint32_t *)in_data, iot_addr, CAM_BUF_SIZE, DMA_IOT_RD_CH); //pio, sm, buffer, start_address, length 
        for (uint32_t i = 0 ; i < CAM_BUF_SIZE/sizeof(uint32_t) ; i++) {
            printf("0x%08X\r\n",in_data[i]);
        }
        // increment iot sram's address
        iot_addr = iot_addr + CAM_BUF_SIZE;

    }

}
void free_cam() {
    
    free(in_data);
}
// camera dma config
dma_channel_config get_cam_config(PIO pio, uint32_t sm, uint32_t dma_chan) {
    dma_channel_config c = dma_channel_get_default_config(dma_chan);    
    channel_config_set_read_increment(&c, false);
    channel_config_set_write_increment(&c, true);
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm, false));
    return c;
}


// IoT SRAM:APS1604M-3SQR (AP Memory)
void iot_sram_init(PIO pio, uint32_t sm) {
    // ----------send reset
    pio_sm_put_blocking(pio, sm, 8-1);      // x=8
    pio_sm_put_blocking(pio, sm, 0);        // y=0
    {
        pio_sm_put_blocking(pio, sm, 0xeffeeffe); 
    }
    pio_sm_put_blocking(pio, sm, 8-1);      // x=8
    pio_sm_put_blocking(pio, sm, 0);        // y=0    
    {
        pio_sm_put_blocking(pio, sm, 0xfeeffeef);  
    }
    sleep_ms(1);
    // ----------send qpi enable mode
    pio_sm_put_blocking(pio, sm, 8-1);      // x=8
    pio_sm_put_blocking(pio, sm, 0);        // y=0    
    {
        pio_sm_put_blocking(pio, sm, 0xeeffefef);  
    }
    sleep_ms(1);
}

void iot_sram_write(PIO pio, uint32_t sm, uint32_t *send_data, uint32_t address, 
        uint32_t length_in_byte, uint32_t dma_channel) {

    uint32_t *b;
    uint32_t num_of_tran_byte; // number of trans at once.(upto 1024bytes)
    b = send_data;
    dma_channel_config c = dma_channel_get_default_config(dma_channel);    
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm, true));
    pio_sm_set_enabled(pio, sm, true);

    while (true) {

        // IoT SRAM needs refresh between writing 1024bytes
        if (length_in_byte > LINEAR_BURST) {
            num_of_tran_byte = LINEAR_BURST;
            length_in_byte = length_in_byte - LINEAR_BURST;
        } else {
            num_of_tran_byte = length_in_byte;
            length_in_byte = 0;
        }
    
        dma_channel_configure(dma_channel, &c,
            &pio->txf[sm],          // Destination pointer
            b,                      // Source pointer
            num_of_tran_byte/4,     // Number of transfers
            false                   // Do not Start yet.
        );

        pio_sm_put_blocking(pio, sm, 8+(num_of_tran_byte*2)-1);         // x=8
        pio_sm_put_blocking(pio, sm, 0);                                // y=0 
        pio_sm_put_blocking(pio, sm, 0x38000000 | address);
    
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

void *iot_sram_read(PIO pio, uint32_t sm, uint32_t *read_data, uint32_t address, 
        uint32_t length_in_byte, uint32_t dma_channel) {
   
    uint32_t *b;
    b = read_data;
    uint32_t num_of_tran_byte = 0; // number of trans at once.(upto LINEAR_BURST bytes)
    dma_channel_config c = dma_channel_get_default_config(dma_channel);    
    channel_config_set_read_increment(&c, false);
    channel_config_set_write_increment(&c, true);
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm, false));

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
            &pio->rxf[sm],          // Source pointer
            num_of_tran_byte/4,     // Number of transfers
            false                   // do not start yet.
        );

        pio_sm_set_enabled(pio, sm, true);
        pio_sm_put_blocking(pio, sm, 8-1);                          // x counter: comm + addr
        pio_sm_put_blocking(pio, sm, (num_of_tran_byte * 2)-1);     // y counter: up to 512byte    

        pio_sm_put_blocking(pio, sm, 0xEB000000 | address);         // send write command + address
        
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

void cam_handler() {
    static uint32_t iot_addr = 0;
    static uint32_t num_of_call_this = 0;
    static uint32_t* b;
    
    //printf("enter %d\r\n",num_of_call_this);
    
     // write iot sram
    if(0 == num_of_call_this % 2) {
        // even
        b = in_data; 
    } else{ 
        //odd
        b = in_data2;
    }
    iot_sram_write(pio_ram, sm_ram, b, iot_addr, CAM_BUF_HALF, DMA_IOT_WR_CH); //pio, sm, buffer, start_address, length
    
    // increment iot sram's address
    iot_addr = iot_addr + CAM_BUF_HALF;

    // clear interrupt flag
    switch(num_of_call_this) {

        case 0:
            dma_hw->ints0 = 1u << DMA_CAM_RD_CH0;
            num_of_call_this++;
            break;
        case 1:
            dma_hw->ints0 = 1u << DMA_CAM_RD_CH1;
            num_of_call_this++;
            break;
        case 2:
            dma_hw->ints0 = 1u << DMA_CAM_RD_CH2;
            num_of_call_this++;
            break;
        case 3:
            dma_hw->ints0 = 1u << DMA_CAM_RD_CH3;
            num_of_call_this++;
            break;
        case 4:
            dma_hw->ints0 = 1u << DMA_CAM_RD_CH4;
            num_of_call_this++;
            break;
        case 5:
            dma_hw->ints0 = 1u << DMA_CAM_RD_CH5;
            num_of_call_this = 0;
            iot_addr = 0;
            is_captured = true;
            break;
        default:
            num_of_call_this = 0;
            iot_addr = 0;
    }
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

//#endif // USE_CAMERA_SYSTEM