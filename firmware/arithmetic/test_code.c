#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico_fft.h"

void fft_test(void){

    int32_t ar2[64] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 1, 2, 1, 0, 1, 2, 1, 1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 1, 2, 1, 0, 1, 2, 1, 1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    int32_t ai2[64] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int32_t n, nmax;
    int32_t wr2[8];
    int32_t wi2[8];
     
    n = nmax = 8;
    float_t ar3[64] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 1, 2, 1, 0, 1, 2, 1, 1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 1, 2, 1, 0, 1, 2, 1, 1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    float_t ai3[64] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float_t wr3[8];
    float_t wi3[8];
    

    int32_t nowtime = time_us_32();
    printf("float FFT2\r\n");
    pico_fft2(n,nmax,ar3,ai3,wr3,wi3);
    pico_ifft2(n,nmax,ar3,ai3,wr3,wi3);
    nowtime = time_us_32() - nowtime;
    
    // real
    for(int i=0; i < (n * nmax); i+=4){
        printf("ar3[%d]=%lf\t ar3[%d]=%lf\t ar3[%d]=%lf\t ar3[%d]=%lf\r\n",i,ar3[i],i+1,ar3[i+1],i+2,ar3[i+2],i+3,ar3[i+3]);
    }

    // imag
    for(int i=0; i < (n * nmax); i+=4){
        printf("ai3[%d]=%lf\t ai3[%d]=%lf\t ai3[%d]=%lf\t ai3[%d]=%lf\r\n",i,ai3[i],i+1,ai3[i+1],i+2,ai3[i+2],i+3,ai3[i+3]);
    }

    printf("elapsed time = %d[usec]\r\n",nowtime);

    printf("Integer FFT2\r\n");
    nowtime = time_us_32();
    pico_int_fft2(n,nmax,ar2,ai2,wr2,wi2);
    pico_int_ifft2(n,nmax,ar2,ai2,wr2,wi2);
    nowtime = time_us_32() - nowtime;
    
    // real
    for(int i=0; i < (n * nmax); i+=4){
        printf("ar2[%d]=%d\t ar2[%d]=%d\t ar2[%d]=%d\t ar2[%d]=%d\r\n",i,ar2[i],i+1,ar2[i+1],i+2,ar2[i+2],i+3,ar2[i+3]);
    }

    // imag
    for(int i=0; i < (n * nmax); i+=4){
        printf("ai2[%d]=%d\t ai2[%d]=%d\t ai2[%d]=%d\t ai2[%d]=%d\r\n",i,ai2[i],i+1,ai2[i+1],i+2,ai2[i+2],i+3,ai2[i+3]);
    }

    printf("elapsed time = %d[usec]\r\n",nowtime);
    
}