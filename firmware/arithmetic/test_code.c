#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico_fft.h"

void fft_test(void){
 
    float_t ar2[16] = { 0., 0., 0., 0., 0., 1., 2., 1., 0., 1., 2., 1., 0., 0., 0., 0.};
    float_t ai2[16] = { 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
    int32_t n, nmax;
    float_t wr2[4];
    float_t wi2[4];
    n = nmax = 4;
    int32_t nowtime = time_us_32();
    pico_fft2(n,nmax,ar2,ai2,wr2,wi2);
    pico_ifft2(n,nmax,ar2,ai2,wr2,wi2);
    nowtime = time_us_32() - nowtime;
    
    // real
    for(int i=0; i < 16; i++){
        printf("ar2[%d]=%lf\r\n",i,ar2[i]);
    }

    // imag
    for(int i=0; i < 16; i++){
        printf("ai2[%d]=%lf\r\n",i,ai2[i]);
    }

    printf("elapsed time = %d[usec]\r\n",nowtime);
    
}