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

    int32_t nowtime = time_us_32();
    pico_int_fft2(n,nmax,ar2,ai2,wr2,wi2);
    pico_int_ifft2(n,nmax,ar2,ai2,wr2,wi2);

    nowtime = time_us_32() - nowtime;
    
    // real
    for(int i=0; i < (n * nmax); i++){
        printf("ar2[%d]=%d\r\n",i,ar2[i]);
    }

    // imag
    for(int i=0; i < (n * nmax); i++){
        printf("ai2[%d]=%d\r\n",i,ai2[i]);
    }

    printf("elapsed time = %d[usec]\r\n",nowtime);
    
}