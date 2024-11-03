#include <stdio.h>
#include "pico/stdlib.h"
#include <math.h>
#include <string.h>
#include "pico/binary_info.h"
#include "picampinos.pio.h"
// #include "pico_psram.h"
#include "sfe_pico.h"
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
#include "image_process.h"
#include "ser_10base_t.pio.h"
#include "des_10base_t.pio.h"

#if USE_100BASE_FX
#include "sfp_hw.h"
#endif
#define USE_COLOR_IMAGE (0) // 0: Depth Estimate, 1:RGB565

static semaphore_t fcmethod_semp;
// volatile bool ram_ind_read = false; // indicate Read
// volatile bool ram_in_write = false; // now writing image
volatile bool irq_indicate_reset = true;

volatile int32_t psram_access = 0; // write buffer:+=1, read buffer:-=1

// init PIO
static PIO pio_cam = pio0;

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
static uint32_t *cam_ptr1; // 2nd pointer of cam_ptr.
static uint8_t *gray_ptr;  // pointer of gray image.
static uint8_t *pad_ptr;   // 1st pointer of padded image.
static float_t **p1_ptr;   // gradient map
static float_t **q1_ptr;   // gradient map
static float_t **d1_ptr;   // depth map.

// static SemaphoreHandle_t xImageGenBinSemaphore;
dma_channel_config get_cam_config(PIO pio, uint32_t sm, uint32_t dma_chan);
void set_pwm_freq_kHz(uint32_t freq_khz, uint32_t system_clk_khz, uint8_t gpio_num);
void cam_handler();

static void memory_stats()
{
    size_t mem_size = sfe_mem_size();
    size_t mem_used = sfe_mem_used();
    printf("\tMemory pool - Total: 0x%X (%u)  Used: 0x%X (%u) - %3.2f%%\n", mem_size, mem_size, mem_used, mem_used,
           (float)mem_used / (float)mem_size * 100.0);

    size_t max_block = sfe_mem_max_free_size();
    printf("\tMax free block size: 0x%X (%u) \n", max_block, max_block);
}

void init_cam(uint8_t DEVICE_IS)
{
    sfe_pico_alloc_init();

    // Initialize CAMERA
    set_pwm_freq_kHz(20000, SYS_CLK_IN_KHZ, PIN_PWM0); // XCLK 24MHz -> OV5642,OV2640
    sleep_ms(1000);

    sccb_init(DEVICE_IS, I2C1_SDA, I2C1_SCL, true); // sda,scl=(gp26,gp27). see 'sccb_if.c' and 'cam.h'
    sleep_ms(3000);

    uint32_t offset_cam = pio_add_program(pio_cam, &picampinos_program);
    uint32_t sm = 0; // pio_claim_unused_sm(pio_cam, true);

    picampinos_program_init(pio_cam, sm_cam, offset_cam, CAM_BASE_PIN, 11); // VSYNC,HREF,PCLK,D[2:9] : total 11 pins
    pio_sm_set_enabled(pio_cam, sm_cam, false);
    pio_sm_clear_fifos(pio_cam, sm_cam);
    pio_sm_restart(pio_cam, sm_cam);
    pio_sm_set_enabled(pio_cam, sm_cam, true);

    // init DMA
    DMA_CAM_RD_CH0 = dma_claim_unused_channel(true);
    DMA_CAM_RD_CH1 = dma_claim_unused_channel(true);

    // IRQ settings
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH0, false);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH1, false);

    // printf("DMA_CH= %d,%d\n", DMA_CAM_RD_CH0, DMA_CAM_RD_CH1);

    // buffer of camera data is IMG_W * IMG_H * 2 bytes (RGB565 = 16 bits = 2 bytes)
    // camera buffer on PSRAM
    // | -- im1 --| -- im2 -- | gray image1 and 2
    // |----------|-----------|
    // | - pad1 - | - pad2 -- | padded image 1 and 2
    // |----------|-----------|
    // | -------- p1  ------- | real and imag of x-normal1
    // | -------- q1  ------- | real and imag of y-normal1
    // | -------- z1  ------- | real and imag of depth estimation1(reserved)
    // |----------|-----------|

    init_image_process(PAD_H, PAD_W);
    // image1 and 2
    cam_ptr = (uint32_t *)malloc(CAM_FUL_SIZE * sizeof(uint32_t) / 2);
    cam_ptr1 = (uint32_t *)malloc(CAM_FUL_SIZE * sizeof(uint32_t) / 2);
    gray_ptr = (uint8_t *)malloc(CAM_FUL_SIZE * 1 * sizeof(uint8_t));
    // 262144
    //  padded image 1 and 2
    //  normal map1 and depth map1
    pad_ptr = (uint8_t *)malloc((PAD_H * PAD_W) * sizeof(uint8_t));
    p1_ptr = alloc_2d_float(PAD_H, PAD_W * 2);
    q1_ptr = alloc_2d_float(PAD_H, PAD_W * 2);
    d1_ptr = alloc_2d_float(PAD_H, PAD_W * 2);

    if (!cam_ptr || !gray_ptr || !cam_ptr1 || !pad_ptr || !p1_ptr || !q1_ptr || !d1_ptr)
    {
        printf("Big block built in allocation failed\n");
        // return 1;
    }

    // todo: check psram size
    memory_stats();
    // init semaphore
    sem_init(&fcmethod_semp, 1, 1);
    sem_release(&fcmethod_semp);
}

void config_cam_buffer()
{
    // ------------------ CAMERA READ: withDMA   --------------------------------

    // disable IRQ
    irq_set_enabled(DMA_IRQ_0, false);

    // (2) 1st DMA Channel Config
    dma_channel_config c;
    c = get_cam_config(pio_cam, sm_cam, DMA_CAM_RD_CH1);
    // trigger DMA_CAM_RD_CH0 when DMA_CAM_RD_CH1 completes. (ping-pong)
    channel_config_set_chain_to(&c, DMA_CAM_RD_CH0);
    dma_channel_configure(DMA_CAM_RD_CH1, &c,
                          cam_ptr1,              // Destination pointer(back half of buffer)
                          &pio_cam->rxf[sm_cam], // Source pointer
                          CAM_FUL_SIZE / 2,      // Number of transfers
                          false                  // Don't Start yet
    );

    // (1) 0th DMA Channel Config
    c = get_cam_config(pio_cam, sm_cam, DMA_CAM_RD_CH0);
    // trigger DMA_CAM_RD_CH1 when DMA_CAM_RD_CH0 completes.
    channel_config_set_chain_to(&c, DMA_CAM_RD_CH1);
    dma_channel_configure(DMA_CAM_RD_CH0, &c,
                          cam_ptr,               // Destination pointer(front half of buffer)
                          &pio_cam->rxf[sm_cam], // Source pointer
                          CAM_FUL_SIZE / 2,      // Number of transfers
                          false                  // Don't Start yet
    );

    // IRQ settings
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH1, true);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH0, true);
    irq_set_exclusive_handler(DMA_IRQ_0, cam_handler);
    irq_set_enabled(DMA_IRQ_0, true);
}

void calc_image(void)
{
    // 光源推定
    float L[3] = {0.2, 1, 1};
    float k;
    uint32_t *b;
    b = (psram_access == 0) ? cam_ptr : cam_ptr1;

    extract_green_from_uint32_array(b, gray_ptr, CAM_FUL_SIZE / 2); // 2つのRGB565(16bit)を32bitパッキングされたデータから2つ分のGreen(uint8_t[])データを取得している
    zeroPadImage(gray_ptr, pad_ptr, IMG_W, IMG_H, 1, PAD_W, PAD_H); // ゼロパディング

    // estimate_lightsource_and_normal(PAD_W, PAD_H, pad_ptr, p1_ptr, q1_ptr, L, &k);
    estimate_normal(PAD_W, PAD_H, pad_ptr, p1_ptr, q1_ptr, L);

    // セマフォの取得
    sem_acquire_blocking(&fcmethod_semp);
    {
        // タスク排他処理
        fcmethod(PAD_W, PAD_H, p1_ptr, q1_ptr, d1_ptr);

        // タスク処理が完了したらセマフォを解放
        sem_release(&fcmethod_semp);
    }

    /*
    // printf()で深度を確認したい場合はここのコメントアウトを解除
        printf("depth = [");
        for (int i = 0; i < IMG_W; i++)
        {
            for (int j = 0; j < IMG_W; j++)
            {
                // int index = i * IMG_W + j;
                printf("%.2f,", d1_ptr[i][2 * j]); // 実数部のみ抽出
            }
            printf("\n");
        }
        printf("];\n");
    */
    printf(".\n");
}

void start_cam()
{

    // Start DMA
    dma_channel_abort(DMA_CAM_RD_CH0);
    dma_start_channel_mask(1u << DMA_CAM_RD_CH0);

    // camera transfer settings(for video)
    pio_sm_put_blocking(pio_cam, sm_cam, 0);                      // X=0 : reserved
    pio_sm_put_blocking(pio_cam, sm_cam, (CAM_FUL_SIZE / 2 - 1)); // Y: total words in an image
}

void uartout_cam()
{
    // read Image
    printf("!srt\r\n");
    sleep_ms(30);

    int32_t *b;
    b = (psram_access == 0) ? cam_ptr1 : cam_ptr;

    for (uint32_t h = 0; h < IMG_H; h++)
    {
        for (uint32_t i = 0; i < (IMG_W / 2); i++)
        {
            printf("0x%08X\r\n", b[(h * (IMG_W / 2)) + i]);
        }
    }
}

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
        uint32_t a[4] = {0xdeadbeef, IMG_H, IMG_W * 2 / sizeof(uint32_t), IMG_H};

        sfp_send(&a, sizeof(uint32_t) * 4);

        // sem_release(&psram_sem);
        for (uint32_t i = 0; i < CAM_FUL_SIZE / sizeof(uint32_t); i += (IMG_W / 2))
        {
            // printf("0x%08X\r\n",b[i]);
            sfp_send_with_header(0xbeefbeef, (i / (IMG_W / 2)) + 1, 1, (IMG_W / 2), &(b[i]), sizeof(uint32_t) * (IMG_W / 2));
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

void rj45_cam(void)
{

    uint8_t udp_payload1[DEF_UDP_PAYLOAD_SIZE] = {0};
    uint32_t tx_buf_udp1[DEF_UDP_BUF_SIZE + 1] = {0};
#if (USE_COLOR_IMAGE)

    uint32_t *b;
    uint32_t resp;
    // RGB565のデータの場合
    b = cam_ptr;

    // send header
    // frame start:
    // '0xdeadbeef' + row_size_in_words(unit is in words(not bytes)) + column_size_in_words(total blocks per frame)
    uint32_t a[4] = {0xdeadbeef, IMG_H, IMG_W / 2, IMG_H};

    // make image header
    udp_packet_gen_10base(tx_buf_udp1, (uint8_t *)&a);

    // send image header
    eth_tx_data(tx_buf_udp1, DEF_UDP_BUF_SIZE);

    for (uint32_t i = 0; i < CAM_FUL_SIZE / 2; i += (IMG_W / 2))
    {
        // printf("0x%08X\r\n",b[i]);
        uint32_t c[] = {
            0xbeefbeef,
            (i / (IMG_W / 2)) + 1,
            1,
            (IMG_W / 2)};

        memcpy(udp_payload1, c, 4 * sizeof(uint32_t));
        memcpy(udp_payload1 + 4 * sizeof(uint32_t), b, sizeof(int32_t) * (IMG_W / 2));
        b += (IMG_W / 2);
        udp_packet_gen_10base(tx_buf_udp1, udp_payload1);
        eth_tx_data(tx_buf_udp1, DEF_UDP_BUF_SIZE);
    }

    a[0] = 0xdeaddead;
    // make image header
    udp_packet_gen_10base(tx_buf_udp1, (uint8_t *)&a);

    // send image header
    eth_tx_data(tx_buf_udp1, DEF_UDP_BUF_SIZE);

#else
    // Float型の場合
    float_t *b;

    // send header
    // frame start:
    // '0xdeadbeef' + row_size_in_words(unit is in words(not bytes)) + column_size_in_words(total blocks per frame)
    uint32_t a[4] = {0xdeadbeef, IMG_H, IMG_W, IMG_H};

    // セマフォの取得。できなかったら待たずに退散。
    if (sem_try_acquire(&fcmethod_semp))
    {
        sem_release(&fcmethod_semp); // タスク完了を待たずにセマフォを解放

        // make image header
        udp_packet_gen_10base(tx_buf_udp1, (uint8_t *)&a);

        // send image header
        eth_tx_data(tx_buf_udp1, DEF_UDP_BUF_SIZE);

        for (uint32_t i = 0; i < IMG_H; i++)
        {
            uint32_t c[] = {
                0xbeefbeef,
                i + 1,
                1,
                (IMG_W)};

            memcpy(udp_payload1, c, 4 * sizeof(uint32_t));
            // ヘッダサイズ分、ポインタをずらす
            uint8_t *st_pos8 = udp_payload1 + 4 * sizeof(int32_t);
            float_t *st_posfl;
            st_posfl = (float_t *)st_pos8;
            for (int j = 0; j < IMG_W; j++)
            {
                float_t data = d1_ptr[i][2 * j];
                memcpy(st_posfl, &data, sizeof(float_t)); //
                st_posfl++;
                // printf("st_posfl=0x%x\n", st_posfl);
            }
            udp_packet_gen_10base(tx_buf_udp1, udp_payload1);
            eth_tx_data(tx_buf_udp1, DEF_UDP_BUF_SIZE);
        }

        a[0] = 0xdeaddead;
        // make image header
        udp_packet_gen_10base(tx_buf_udp1, (uint8_t *)&a);

        // send image header
        eth_tx_data(tx_buf_udp1, DEF_UDP_BUF_SIZE);
    }
#endif
}

void free_cam()
{

    // Disable IRQ settings
    irq_set_enabled(DMA_IRQ_0, false);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH1, false);
    dma_channel_set_irq0_enabled(DMA_CAM_RD_CH0, false);
    dma_channel_abort(DMA_CAM_RD_CH1);
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
    static uint32_t num_of_call_this = 0;
    static uint32_t *b;
    uint32_t dma_chan;

    if (num_of_call_this > 0)
        return;
    num_of_call_this++;

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
        psram_access = 0;
        b = cam_ptr;
        gpio_put(25, 1);
    }

    if (triggered_dma & (1u << DMA_CAM_RD_CH1))
    {
        // Triggered by DMA_CAM_RD_CH1
        // printf("DMA channel 1 triggered the interrupt.\n");
        dma_chan = DMA_CAM_RD_CH1;
        psram_access = 1;
        b = cam_ptr1;
        gpio_put(25, 0);
    }

    // clear interrupt flag
    dma_hw->ints0 = triggered_dma;

    // reset the DMA initial write address
    dma_channel_set_write_addr(dma_chan, b, false);

    num_of_call_this = 0;
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

void vImageProc(void *pvParameters)
{
    printf("vImageProc - Running on Core: %d\n", get_core_num()); // 現在のコア番号を表示

    while (1)
    {
        calc_image();
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}