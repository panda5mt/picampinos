#include <stdio.h>
#include <stdlib.h>

void estimate_lightsource_and_normal(int width, int height,
                                     unsigned char *img_gray, float *p, float *q,
                                     float *L, float *absL);

int32_t fcmethod(int width, int height,
                 float *p, float *ip,
                 float *q, float *iq,
                 float *Z_real, float *Z_imag);

int find_minimum_n(int k);

// Function to perform zero-padding on an image
void zeroPadImage(const unsigned char *input,
                  unsigned char **output,
                  int originalWidth, int originalHeight, int channels,
                  int paddedWidth, int paddedHeight);
