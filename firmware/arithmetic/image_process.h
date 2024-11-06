#include <stdio.h>
#include <stdlib.h>
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

void estimate_lightsource_and_normal(int width, int height,
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
void estimate_normal(int width, int height, unsigned char *img_gray, float **p, float **q, float *L);

// Frankot-Chellappa algorithm(We need just *Z_real)
int32_t fcmethod(int width, int height,
                 float **p, float **q,
                 float **dp);

// Function to perform zero-padding on an image
void zeroPadImage(const unsigned char *input,
                  unsigned char *output,
                  int originalWidth, int originalHeight, int channels,
                  int paddedWidth, int paddedHeight);

// extract green from RGB565 packed data.
// length: input's length. you will need output[2*length]
// (length = sizeof(input[array])/sizeof(input[0]) )
void extract_green_from_uint32_array(const uint32_t *input, uint8_t *output, size_t length);