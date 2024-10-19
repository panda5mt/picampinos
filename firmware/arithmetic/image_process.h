#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*

compiler infomation: You should same compiler version when rebuild this code.
# arm-none-eabi-gcc -v

Using built-in specs.
COLLECT_GCC=arm-none-eabi-gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/arm-none-eabi/12.2.1/lto-wrapper
*/
void estimate_lightsource_and_normal(int width, int height,
                                     unsigned char *img_gray, float *p, float *q,
                                     float *L, float *absL);

int32_t fcmethod(int width, int height,
                 float *p, float *ip,
                 float *q, float *iq,
                 float *Z_real, float *Z_imag);

// Function to perform zero-padding on an image
void zeroPadImage(const unsigned char *input,
                  unsigned char *output,
                  int originalWidth, int originalHeight, int channels,
                  int paddedWidth, int paddedHeight);

// extract green from RGB565 packed data.
// length: input's length. you will need output[2*length]
// (length = sizeof(input[array])/sizeof(input[0]) )
void extract_green_from_uint32_array(const uint32_t *input, uint8_t *output, size_t length);