// PICAMPINOS - Camera IF using RP2040
// this project communicates OV5642(or parallel 8-bit DVP-IF Image sensor with SCCB control) using Raspberry Pi PICO
// Need: A Raspberry Pi PICO, An IoT SRAM(APS1604M-3SQR or compatible), A DVP Camera(OV5642)
// Connection:
// ----- with OV Camera -----
// You can reverse bits (GP1:GP8=D2:D9 -> GP1:GP8=D9:D2). search for keywords 'bit reverse' in 'picampinos.pio'
// (PICO <-> OV5642/OV2640)
// GP0 ---> XCLK(24MHz Clock IN)
// GP1 <--- D2 
// GP2 <--- D3
// GP3 <--- D4
// GP4 <--- D5
// GP5 <--- D6
// GP6 <--- D7
// GP7 <--- D8
// GP8 <--- D9
// GP9 <--- VSYNC
// GP10 <-- HREF(HSYNC)
// GP11 <-- PCLK(Pixel Clock OUT)

// GP12~GP15 : reserved for SPI. (SPI: See also ezspi_slave.c if you want to use.)

// GP26 --> SDA(SCCB DATA/I2C)
// GP27 --> SCL(SCCB CLK/I2C)

// ----- with IoT SRAM -----
// (PICO <-> APS1604M)
// GP16 <-> D0
// GP17 <-> D1
// GP18 <-> D2
// GP19 <-> D3
// GP20 --> xCS
// GP21 --> SCLK

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "cam.h"
#include "arithmetic/my_fft.h"

#define BOARD_LED           (25) // pico's led => 25, self made RP2040brd's led => 28. check hardware/RP2040Board.pdf 


static PIO pio_ser_wr = pio1;
static uint sm0;

static void read_i2c_data(i2c_inst_t *i2c)
{
    uint8_t read_buf[256] = {0};
    uint32_t x = 0, y = 0;
    uint8_t tmp;
    printf("read sfp start...\r\n");

    // sfp's addr = 0xA0
    i2c_read_blocking(i2c, (0xA0 >> 1), read_buf, 256, false);

    printf("read sfp end...\r\n");
    printf(" 0x50 0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef\r\n");
    for (y = 0; y < 16; y++) {
        printf(" %02x: ", y << 4);
        for (x = 0; x < 16; x++) {
            printf("%02x ", read_buf[(y << 4) + x]);
        }
        printf("   ");
        for (x = 0; x < 16; x++) {
            tmp = read_buf[(y << 4) + x];
            if (tmp >= 0x20 && tmp <= 0x7E) {
                printf("%c", tmp);
            } else {
                printf(".");
            }
        }
        printf("\r\n");
    }
}

bool setup() {
    bool OC_INIT = false;
    vreg_set_voltage(VREG_VOLTAGE_1_30);
    // system init
    stdio_init_all();
    OC_INIT = set_sys_clock_khz(SYS_CLK_KHZ, true);
    setup_default_uart();
    

    // INIT LED
    gpio_init(BOARD_LED);
    gpio_set_dir(BOARD_LED, GPIO_OUT);
    for(int i=0 ; i < 5; i++) {     // blink LED
        gpio_put(BOARD_LED, 1);
        busy_wait_ms(100);
        gpio_put(BOARD_LED, 0);
        busy_wait_ms(100);
    }

    return OC_INIT;
    //gpio_pull_up(BOARD_LED);

}

//#define		CPX		/* Complex mode */

#ifndef CPX
void print(double *ar, double *ai, int n);
void print2(double *ar, double *ai, int n, int nmax);
#else
void printx(Complex *a, int n);
void print2x(Complex *a, int n, int nmax);
#endif

#ifndef CPX
void print(double *ar, double *ai, int n)
{
	int i;
	double *p, *q;

	printf("    ar              ai\n");
	for(i = 0, p = ar, q = ai; i < n; i++)	printf("%14.7e %14.7e\n", *p++, *q++);
}
#else
void printx(Complex *a, int n)
{
	int i;
	Complex *p;

	printf("    ar              ai\n");
	for(i = 0, p = a; i < n; i++, p++)	printf("%14.7e %14.7e\n", p->r, p->i);
}
#endif
#ifndef CPX
void print2(double *ar, double *ai, int n, int nmax)
{
	int i, j, k, l;
	double *p;

	printf("< ar >\n");
	for(i = k = 0, p = ar; i < n; i++)
	{
		for(j = 0; j < nmax; j++, k++)	printf("%14.7e ", *p++);
		putchar('\n');
	}
	printf("< ai >\n");
	for(i = k = 0, p = ai; i < n; i++)
	{
		for(j = 0; j < nmax; j++, k++)	printf("%14.7e ", *p++);
		putchar('\n');
	}
}
#else
void print2x(Complex *a, int n, int nmax)
{
	int i, j, k, l;
	Complex *p;

	printf("< Real part >\n");
	for(i = k = 0, p = a; i < n; i++)
	{
		for(j = 0; j < nmax; j++, k++, p++)	printf("%14.7e ", p->r);
		putchar('\n');
	}
	printf("< Imaginary part >\n");
	for(i = k = 0, p = a; i < n; i++)
	{
		for(j = 0; j < nmax; j++, k++, p++)	printf("%14.7e ", p->i);
		putchar('\n');
	}
}
#endif


int main() {

    bool OC_OK = false;
    OC_OK = setup();
    if(OC_OK) {      
        printf("clock init okay.\r\n");
        sleep_ms(5000);
    }else{
        printf("clock init failed.\r\n");
        sleep_ms(5000);
    }
    /// 

    uint32_t nowtime ;
   
    static double ar[8] = { 0., 0., 0., 1., 1., 0., 0., 0.};
	static double ai[8] = { 0., 0., 0., 0., 0., 0., 0., 0.};
	static double ar2[16] = { 0., 0., 0., 0., 0., 1., 1., 0.,
							  0., 1., 1., 0., 0., 0., 0., 0.};
	static double ai2[16] = { 0., 0., 0., 0., 0., 0., 0., 0.,
							  0., 0., 0., 0., 0., 0., 0., 0.};
	Complex a[8], a2[16];
	int flag, i, iter, j, k, n, nmax;

	iter = 0;
	n = 8;
    for(int zzz=0; zzz<5; zzz++ ){
        nowtime = time_us_32();
        #ifndef CPX
        for(i = 0; i < n; i++)	a[i] = tocomplex(ar[i], ai[i]);
        //print(ar, ai, n);
        #else
        printx(a, n);
        #endif

        /* forward FFT */
        flag = 0;
        #ifndef CPX
        fft1(ar, ai, n, iter, flag);
        //print(ar, ai, n);
        #else
        fft1x(a, n, iter, flag);
        printx(a, n);
        #endif
        //nowtime = time_us_32() - nowtime;
        //printf("elapsed time = %ld\r\n",nowtime);

        //nowtime = time_us_32();
        /* reverse FFT */
            flag = 1;
        #ifndef CPX
            fft1(ar, ai, n, iter, flag);
            //print(ar, ai, n);
        #else
            fft1x(a, n, iter, flag);
            printx(a, n);
        #endif

            n = nmax = 4;
        for(i = k = 0; i < n; i++)	for(j = 0; j < n; j++, k++)	a2[k] = tocomplex(ar2[k], ai2[k]);

    #ifndef CPX
        //print2(ar2, ai2, n, nmax);
    #else
        print2x(a2, n, nmax);
    #endif
        flag = 0;
    #ifndef CPX
        fft2(ar2, ai2, n, nmax, flag);
        //print2(ar2, ai2, n, nmax);
    #else
        fft2x(a2, n, nmax, flag);
        print2x(a2, n, nmax);
    #endif
        flag = 1;
    #ifndef CPX
        fft2(ar2, ai2, n, nmax, flag);
        //print2(ar2, ai2, n, nmax);
    #else
        fft2x(a2, n, nmax, flag);
        print2x(a2, n, nmax);
    #endif

        nowtime = time_us_32() - nowtime;
        printf("elapsed time = %ld\r\n",nowtime);
    }
    while(1);
    /// 

    init_cam(DEV_OV5642);
    config_cam_buffer();    // config buffer
    start_cam();            // start streaming

    // call 'read_i2c_data()' after 'init_cam()' because i2c hardware is not initialized before 'init_cam()'
    i2c_inst_t *i2c = i2c1;
    read_i2c_data(i2c);


    
    // data via USB-UART(ASCII)
    // see also 'matlab/readrgb.m'    
    // while(true) {
    //     uartout_cam();          
    // }

    // data via USB-UART(binary) 
    // see also 'matlab/comm_uart_bin.m'
    /*    
    while(true) {
        uartout_bin_cam();      
    }                           
    */

    // you have Raspberry Pi 3/4? and you have MATLAB?
    // you can use SPI + RPi + MATLAB
    // and CHECK 'USE_EZSPI_SLAVE' is (true) in 'cam.h'
    // See also 'matlab/comm_raspi_spi.m'
    /*
    while(true) {
        spiout_cam();
        printf("OK\r\n");
    }
    */
    
    // using SFP module
    // see also 'matlab/receive_udp.m'
    // To Use SFP, CHECK 'USE_EZSPI_SLAVE' is (false) 
    // AND 'USE_100BASE_FX' is (true) in 'cam.h'
    while(true) {
        sfp_cam();
    }
    
    // end
    free_cam();

    while (true);
}


