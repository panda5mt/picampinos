#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "test_code.h"

void dct_test(int num){
    // num : number of calc dct
    uint8_t img[][16] = {
        {139, 144, 149, 153, 155, 155, 155, 155, 144, 151, 153, 156, 159, 156, 156, 156},
        {150, 155, 160, 163, 158, 156, 156, 156, 159, 161, 162, 160, 160, 159, 159, 159},
        {159, 160, 161, 162, 162, 155, 155, 155, 161, 161, 161, 161, 160, 157, 157, 157},
        {162, 162, 161, 163, 162, 157, 157, 157, 162, 162, 161, 161, 163, 158, 158, 158},
        {139, 144, 149, 153, 155, 155, 155, 155, 144, 151, 153, 156, 159, 156, 156, 156},
        {150, 155, 160, 163, 158, 156, 156, 156, 159, 161, 162, 160, 160, 159, 159, 159},
        {159, 160, 161, 162, 162, 155, 155, 155, 161, 161, 161, 161, 160, 157, 157, 157},
        {162, 162, 161, 163, 162, 157, 157, 157, 162, 162, 161, 161, 163, 158, 158, 158},
        {139, 144, 149, 153, 155, 155, 155, 155, 144, 151, 153, 156, 159, 156, 156, 156},
        {150, 155, 160, 163, 158, 156, 156, 156, 159, 161, 162, 160, 160, 159, 159, 159},
        {159, 160, 161, 162, 162, 155, 155, 155, 161, 161, 161, 161, 160, 157, 157, 157},
        {162, 162, 161, 163, 162, 157, 157, 157, 162, 162, 161, 161, 163, 158, 158, 158},
        {139, 144, 149, 153, 155, 155, 155, 155, 144, 151, 153, 156, 159, 156, 156, 156},
        {150, 155, 160, 163, 158, 156, 156, 156, 159, 161, 162, 160, 160, 159, 159, 159},
        {159, 160, 161, 162, 162, 155, 155, 155, 161, 161, 161, 161, 160, 157, 157, 157},
        {162, 162, 161, 163, 162, 157, 157, 157, 162, 162, 161, 161, 163, 158, 158, 158}
    };


    int32_t img_w = 16;
    int32_t nowtime;
    int32_t img_h = sizeof(img) / sizeof(uint8_t) / img_w;
    uint8_t *np = (uint8_t *)img;       // 1次元配列に変換
    float_t coeff[img_h * img_w];       // 1次元配列で宣言
    int32_t img_idct[img_h * img_w];    // 1次元配列で宣言
    // DCTテーブルの作成
    make_dct_table();

    for(int n = 0; n < num; n++) {
        nowtime = time_us_32();
        pico_dct8(np, coeff, img_h, img_w); 

        nowtime = time_us_32() - nowtime;
        printf("DCT:elapsed time = %d[us]\r\n",nowtime);

        // 結果を出力 
        for(int32_t h = 0 ; h < img_h ; h++) {
            for(int32_t w = 0 ; w < img_w ; w++) {
                printf("%8.2f",coeff[h*img_w + w]);
            }
            printf("\n");
        }
        printf("------\n");

        nowtime = time_us_32();
        pico_idct8(coeff, img_idct, img_h, img_w);
        nowtime = time_us_32() - nowtime;
        printf("IDCT:elapsed time = %d[us]\r\n",nowtime);
        // 結果を出力 
        for(int32_t h = 0 ; h < img_h ; h++) {
            for(int32_t w = 0 ; w < img_w ; w++) {
                printf("%6d",img_idct[h*img_w + w]);
            }
            printf("\n");
        }
        printf("------\n");
    }
}

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
    
    // // real
    // for(int i=0; i < (n * nmax); i+=4){
    //     printf("ar3[%d]=%lf\t ar3[%d]=%lf\t ar3[%d]=%lf\t ar3[%d]=%lf\r\n",i,ar3[i],i+1,ar3[i+1],i+2,ar3[i+2],i+3,ar3[i+3]);
    // }

    // // imag
    // for(int i=0; i < (n * nmax); i+=4){
    //     printf("ai3[%d]=%lf\t ai3[%d]=%lf\t ai3[%d]=%lf\t ai3[%d]=%lf\r\n",i,ai3[i],i+1,ai3[i+1],i+2,ai3[i+2],i+3,ai3[i+3]);
    // }

    printf("elapsed time = %d[usec]\r\n",nowtime);

    printf("Integer FFT2\r\n");
    nowtime = time_us_32();
    pico_int_fft2(n,nmax,ar2,ai2,wr2,wi2);
    pico_int_ifft2(n,nmax,ar2,ai2,wr2,wi2);
    nowtime = time_us_32() - nowtime;
    
    // // real
    // for(int i=0; i < (n * nmax); i+=4){
    //     printf("ar2[%d]=%d\t ar2[%d]=%d\t ar2[%d]=%d\t ar2[%d]=%d\r\n",i,ar2[i],i+1,ar2[i+1],i+2,ar2[i+2],i+3,ar2[i+3]);
    // }

    // // imag
    // for(int i=0; i < (n * nmax); i+=4){
    //     printf("ai2[%d]=%d\t ai2[%d]=%d\t ai2[%d]=%d\t ai2[%d]=%d\r\n",i,ai2[i],i+1,ai2[i+1],i+2,ai2[i+2],i+3,ai2[i+3]);
    // }

    printf("elapsed time = %d[usec]\r\n",nowtime);
    
}

