#include <stdio.h>
#include <stdbool.h>
#include "fft4f2d.h"

/*

compiler infomation: You should same compiler version when rebuild this code.
# arm-none-eabi-gcc -v

Using built-in specs.
COLLECT_GCC=arm-none-eabi-gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/arm-none-eabi/12.2.1/lto-wrapper

*/

#define USE_REAL_FFT (true) // do not change

void init_image_process(int height, int width);

void estimate_lightsource_and_normal(int height, int width,
                                     unsigned char *img_gray, float **p, float **q,
                                     float *L, float *absL);

// Use estimate_normal() if you already have the light source position(L*).
// this is faster than estimate_lightsource_and_normal().
//
// input:
// width, height
// img_gray(uint8_t 1-D array)
// *L: lightsource (L[3] = {Lx,Ly,Lz};)
//
// output:
// p: normal map(X)
// q: normal map(Y)
void estimate_normal(int height, int width, unsigned char *img_gray, float **p, float **q, float *L);

// Frankot-Chellappa algorithm(We need just *Z_real)
int32_t fcmethod(int height, int width,
                 float **p, float **q,
                 float **dp);
