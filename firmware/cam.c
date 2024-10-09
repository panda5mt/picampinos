#include <stdio.h>
#include "pico/stdlib.h"
#include <math.h>
#include "pico/binary_info.h"
#include "picampinos.pio.h"
// #include "class/cdc/cdc_device.h" // for uart(binary output)
#include "pico_psram.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"
#include "hardware/dma.h"
#include "hardware/pio.h"

#include "hardware/clocks.h"
#include "pico/multicore.h"

#include "cam.h"

#include "system.h"
#include "eth.h"
#include "arp.h"
#include "icmp.h"
#include "udp.h"
#include "ser_10base_t.pio.h"
#include "des_10base_t.pio.h"

#if USE_EZSPI_SLAVE
#include "ezspi_slave.h"
#endif
#if USE_100BASE_FX
#include "sfp_hw.h"
#endif

// PSRAM START ADDRESS
#define PSRAM_LOCATION _u(0x11000000)

volatile bool ram_ind_read = false; // indicate Read
volatile bool ram_in_write = false; // now writing image

volatile bool irq_indicate_reset = true;

volatile int32_t psram_access = 0; // write buffer:+=1, read buffer:-=1
static uint32_t tx_buf_udp1[DEF_UDP_BUF_SIZE + 1] = {0};

// init PIO
static PIO pio_cam = pio0;

#if USE_EZSPI_SLAVE
PIO pio_spi = pio1; // same PIO with pio_iot
#endif
#if USE_100BASE_FX
PIO pio_sfp = pio0; // same PIO with pio_iot
#endif

// statemachine's pointer
static uint32_t sm_cam; // CAMERA's state machines

// dma channels
static uint32_t DMA_CAM_RD_CH0;
static uint32_t DMA_CAM_RD_CH1;

// private functions and buffers
static uint32_t *cam_ptr;  // pointer of camera buffer
static uint32_t *cam_ptr2; // 2nd pointer of cam_ptr.
// uint32_t *iot_ptr;  // pointer of IoT RAM's read buffer.

dma_channel_config get_cam_config(PIO pio, uint32_t sm, uint32_t dma_chan);
void set_pwm_freq_kHz(uint32_t freq_khz, uint32_t system_clk_khz, uint8_t gpio_num);
void cam_handler();
// void printbuf(uint8_t buf[], size_t len);

void init_cam(uint8_t DEVICE_IS)
{

    // Initialize CAMERA
    set_pwm_freq_kHz(24000, SYS_CLK_IN_KHZ, PIN_PWM0); // XCLK 24MHz -> OV5642,OV2640
    sleep_ms(1000);

    sccb_init(DEVICE_IS, I2C1_SDA, I2C1_SCL, true); // sda,scl=(gp26,gp27). see 'sccb_if.c' and 'cam.h'
    sleep_ms(3000);

    uint32_t offset_cam = pio_add_program(pio_cam, &picampinos_program);
    uint32_t sm = 0; // pio_claim_unused_sm(pio_cam, true);
    // printf("camera:pio=%d, sm = %d, offset=%d\r\n", (uint32_t)pio_cam, sm, offset_cam);

    picampinos_program_init(pio_cam, sm_cam, offset_cam, CAM_BASE_PIN, 11); // VSYNC,HREF,PCLK,D[2:9] : total 11 pins
    pio_sm_set_enabled(pio_cam, sm_cam, false);
    pio_sm_clear_fifos(pio_cam, sm_cam);
    pio_sm_restart(pio_cam, sm_cam);
    pio_sm_set_enabled(pio_cam, sm_cam, true);

    // Initialize IoT SRAM
    // iot_sram_init(pio_iot);

    // init DMA
    DMA_CAM_RD_CH0 = dma_claim_unused_channel(true);
    DMA_CAM_RD_CH1 = dma_claim_unused_channel(true);

    // IRQ settings
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH0, false);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH1, false);

    printf("DMA_CH= %d,%d\n", DMA_CAM_RD_CH0, DMA_CAM_RD_CH1);

    // buffer of camera data is 640 * 480 * 2 bytes (RGB565 = 16 bits = 2 bytes)
    // camera buffer on PSRAM
    uint32_t *data_buffer = (uint32_t *)(PSRAM_LOCATION);
    cam_ptr = data_buffer;
    data_buffer += CAM_FUL_SIZE / sizeof(uint32_t);
    cam_ptr2 = data_buffer;
    // iot_ptr = cam_ptr;

    // todo: check psram size
}

void config_cam_buffer()
{
    // ------------------ CAMERA READ: withDMA   --------------------------------

    // is_captured = false;

    // check psram

    int sz = pico_setup_psram(PICO_PSRAM_CS1);
    if (sz == 0)
    {
        printf("No PSRAM.\nSystem halted.\n");
        while (1)
            ;
    }
    else
    {
        printf("PSRAM OK:size = %d\n", sz);
    }

    // disable IRQ
    irq_set_enabled(DMA_IRQ_0, false);

    // (2) 1st DMA Channel Config
    dma_channel_config c;
    c = get_cam_config(pio_cam, sm_cam, DMA_CAM_RD_CH1);
    // trigger DMA_CAM_RD_CH0 when DMA_CAM_RD_CH1 completes. (ping-pong)
    channel_config_set_chain_to(&c, DMA_CAM_RD_CH0);
    dma_channel_configure(DMA_CAM_RD_CH1, &c,
                          cam_ptr2,                        // Destination pointer(back half of buffer)
                          &pio_cam->rxf[sm_cam],           // Source pointer
                          CAM_FUL_SIZE / sizeof(uint32_t), // Number of transfers
                          false                            // Don't Start yet
    );

    // (1) 0th DMA Channel Config
    c = get_cam_config(pio_cam, sm_cam, DMA_CAM_RD_CH0);
    // trigger DMA_CAM_RD_CH1 when DMA_CAM_RD_CH0 completes.
    channel_config_set_chain_to(&c, DMA_CAM_RD_CH1);
    dma_channel_configure(DMA_CAM_RD_CH0, &c,
                          cam_ptr,                         // Destination pointer(front half of buffer)
                          &pio_cam->rxf[sm_cam],           // Source pointer
                          CAM_FUL_SIZE / sizeof(uint32_t), // Number of transfers
                          false                            // Don't Start yet
    );

    // IRQ settings
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH1, true);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH0, true);
    irq_set_exclusive_handler(DMA_IRQ_0, cam_handler);
    //  irq_indicate_reset = true;
    //    sem_init(&psram_sem, 1, 1); // init semaphore
    //    enable IRQ
    //   irq_set_enabled(DMA_IRQ_0, true);
    //   irq_set_priority(DMA_IRQ_0, 0); // Most high
    //   Set IRQ handler
    // irq_add_shared_handler(DMA_IRQ_0, &cam_handler, 127);
    irq_set_enabled(DMA_IRQ_0, true);
}

void start_cam()
{

    // Start DMA
    dma_channel_abort(DMA_CAM_RD_CH0);
    dma_start_channel_mask(1u << DMA_CAM_RD_CH0);

    // camera transfer settings(for video)
    pio_sm_put_blocking(pio_cam, sm_cam, 0);                                     // X=0 : reserved
    pio_sm_put_blocking(pio_cam, sm_cam, (CAM_FUL_SIZE / sizeof(uint32_t) - 1)); // Y: total words in an image

    // wait until transfer finish
    // while(false == is_captured);
}

void uartout_cam()
{
    // read Image
    printf("!srt\r\n");
    sleep_ms(30);

    // is_captured = false;
    // while (!is_captured)
    //     ; // wait until an image captured

    // while (ram_in_write)
    //     ;                // wait until writing ram finished
    // ram_ind_read = true; // start to read

    // int32_t iot_addr = 0;
    int32_t *b;
    b = (psram_access == 0) ? cam_ptr2 : cam_ptr;

    for (uint32_t h = 0; h < 480; h++)
    {
        for (uint32_t i = 0; i < 320; i++)
        {
            printf("0x%08X\r\n", b[(h * 320) + i]);
        }
    }
    // increment iot sram's address
    // iot_addr = cam_ptr2;
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
void spiout_cam()
{
    uint8_t BUF_LEN = 10;
    uint8_t in_buf[BUF_LEN];
    uint8_t out_buf[BUF_LEN];

    is_captured = false;
    sleep_ms(80);

    while (!is_captured)
        ; // wait until an image captured
    while (ram_in_write)
        ;                // wait until writing ram finished
    ram_ind_read = true; // start to read

    init_spi_slave(pio_spi);

    int32_t iot_addr = 0;
    int32_t *b;
    uint32_t resp;
    b = iot_ptr;

    // send header
    write_word_spi_slave(pio_spi, 0xDEADBEEF);
    for (uint32_t h = 0; h < 480; h = h + BLOCK)
    {
        iot_sram_read(pio_iot, (uint32_t *)b, iot_addr, CAM_BUF_SIZE, DMA_IOT_RD_CH); // pio, sm, buffer, start_address, length

        for (uint32_t i = 0; i < CAM_BUF_SIZE / sizeof(uint32_t); i += 640 * 2 / sizeof(uint32_t))
        {
            write_blocking_spi_slave(pio_spi, &b[i], 640 * 2);
        }
        // increment iot sram's address
        iot_addr = iot_addr + CAM_BUF_SIZE;
    }

    // deinit_spi_slave();
    ram_ind_read = false;
}
#endif

#if USE_100BASE_FX
void sfp_cam()
{
    static int32_t iot_addr;
    sfp_hw_init(pio_sfp);

    while (1)
    {

        int32_t *b;
        uint32_t resp;
        b = cam_ptr + iot_addr;

        // send header
        // frame start:
        // '0xdeadbeef' + row_size_in_words(unit is in words(not bytes)) + columb_size_in_words(total blocks per frame)
        uint32_t a[4] = {0xdeadbeef, 480, 640 * 2 / sizeof(uint32_t), 480};

        sfp_send(&a, sizeof(uint32_t) * 4);

        // sem_release(&psram_sem);
        for (uint32_t i = 0; i < CAM_FUL_SIZE / sizeof(uint32_t); i += 320)
        {
            // printf("0x%08X\r\n",b[i]);
            sfp_send_with_header(0xbeefbeef, (i / 320) + 1, 1, 320, &(b[i]), sizeof(uint32_t) * 320);
        }

        // increment iot sram's address
        iot_addr = iot_addr + CAM_FUL_SIZE;

        if (iot_addr > CAM_TOTAL_LEN - 1)
        {
            iot_addr = 0;
        }
        // send dummy data

        a[0] = 0xdeaddead;
        sfp_send(&a, sizeof(uint32_t) * 1);
    }
}
#endif

void rj45_cam()
{
    // static int32_t iot_addr;
    // sfp_hw_init(pio_sfp);

    // while (1)
    {

        int32_t *b;
        uint32_t resp;
        b = cam_ptr; //+ iot_addr;

        // send header
        // frame start:
        // '0xdeadbeef' + row_size_in_words(unit is in words(not bytes)) + columb_size_in_words(total blocks per frame)
        uint32_t a[4] = {0xdeadbeef, 480, 640 * 2 / sizeof(uint32_t), 480};

        // time_udp = time_now;
        uint8_t udp_payload1[DEF_UDP_PAYLOAD_SIZE] = {0};
        sprintf(udp_payload1, "Hello World!! Raspico 10BASE-T !!!!!!%d", time_us_32());
        udp_packet_gen_10base(tx_buf_udp1, (uint8_t *)&udp_payload1);

        // sfp_send(&a, sizeof(uint32_t) * 4);
        // udp_packet_gen_10base(tx_buf_udp1, (uint8_t *)&a);
        //

        eth_tx_data(tx_buf_udp1, DEF_UDP_BUF_SIZE + 1);
        // sem_release(&psram_sem);
        // for (uint32_t i = 0; i < CAM_FUL_SIZE / sizeof(uint32_t); i += 320)
        // {
        //     // printf("0x%08X\r\n",b[i]);
        //     sfp_send_with_header(0xbeefbeef, (i / 320) + 1, 1, 320, &(b[i]), sizeof(uint32_t) * 320);
        // }

        // // increment iot sram's address
        // iot_addr = iot_addr + CAM_FUL_SIZE;

        // if (iot_addr > CAM_TOTAL_LEN - 1)
        // {
        //     iot_addr = 0;
        // }
        // // send dummy data

        // a[0] = 0xdeaddead;
        // sfp_send(&a, sizeof(uint32_t) * 1);
    }
}

void free_cam()
{

    // Disable IRQ settings
    irq_set_enabled(DMA_IRQ_0, false);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH1, false);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH0, false);
    dma_channel_abort(DMA_CAM_RD_CH0);
    dma_channel_abort(DMA_CAM_RD_CH0);
}

/// camera dma config
dma_channel_config get_cam_config(PIO pio, uint32_t sm, uint32_t dma_chan)
{
    dma_channel_config c = dma_channel_get_default_config(dma_chan);
    channel_config_set_read_increment(&c, false);
    channel_config_set_write_increment(&c, true);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_32);
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm, false));
    return c;
}

void cam_handler()
{
    // printf(".");

    static uint32_t num_of_call_this = 0;
    static uint32_t *b;
    uint32_t dma_chan;

    psram_access = psram_access + 1;
    if (psram_access > CAM_TOTAL_FRM)
    {
        psram_access = 0;
    }
    // sem_acquire_blocking(&psram_sem);
    // iot_sram_write(pio_iot, b, iot_addr, CAM_BUF_HALF, DMA_IOT_WR_CH); // pio, sm, buffer, start_address, length
    // sem_release(&psram_sem);

    // increment iot sram's address
    // iot_addr = iot_addr + CAM_BUF_HALF;
    uint32_t triggered_dma = dma_hw->ints0; // DMA_IRQ_0に関連する割り込みステータス

    if (triggered_dma & (1u << DMA_CAM_RD_CH0))
    {
        // Triggered by DMA_CAM_RD_CH0
        // printf("DMA channel 0 triggered the interrupt.\n");
        dma_chan = DMA_CAM_RD_CH0;
        b = cam_ptr;
        gpio_put(25, 1);
    }

    if (triggered_dma & (1u << DMA_CAM_RD_CH1))
    {
        // Triggered by DMA_CAM_RD_CH1
        // printf("DMA channel 1 triggered the interrupt.\n");
        dma_chan = DMA_CAM_RD_CH1;
        b = cam_ptr2;
        gpio_put(25, 0);
    }

    // clear interrupt flag
    dma_hw->ints0 = triggered_dma;

    // reset the DMA initial write address
    dma_channel_set_write_addr(dma_chan, b, false);

    return;
}

//// PWM
void set_pwm_freq_kHz(uint32_t freq_khz, uint32_t system_clk_khz, uint8_t gpio_num)
{

    uint32_t pwm0_slice_num;
    uint32_t period;
    static pwm_config pwm0_slice_config;
    system_clk_khz = clock_get_hz(clk_sys) / 1000;
    period = system_clk_khz / freq_khz - 1;
    if (period < 1)
        period = 1;

    gpio_set_function(gpio_num, GPIO_FUNC_PWM);
    pwm0_slice_num = pwm_gpio_to_slice_num(gpio_num);

    // config
    pwm0_slice_config = pwm_get_default_config();
    pwm_config_set_wrap(&pwm0_slice_config, period);

    // set clk div
    pwm_config_set_clkdiv(&pwm0_slice_config, 1);

    // set PWM start
    pwm_init(pwm0_slice_num, &pwm0_slice_config, true);
    pwm_set_gpio_level(gpio_num, (pwm0_slice_config.top * 0.50)); // duty:50%
}
