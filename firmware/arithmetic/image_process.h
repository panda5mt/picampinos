#include <stdio.h>
#include <stdlib.h>
// #include "pico/stdlib.h"

void estimate_lightsource_with_sobel(int width, int height,
                                     unsigned char *img_gray, float *p, float *q,
                                     float *L, float *absL);

void estimate_normal(int width, int height,
                     unsigned char *img_gray, float *p, float *q,
                     float *L);

int32_t fcmethod(int width, int height,
                 float p[], float ip[],
                 float q[], float iq[],
                 float Z_real[], float Z_imag[]);
