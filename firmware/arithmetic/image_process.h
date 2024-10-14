#include <stdio.h>
#include <stdlib.h>

void estimate_lightsource_and_normal(int width, int height,
                                     unsigned char *img_gray, float *p, float *q,
                                     float *L, float *absL);

int32_t fcmethod(int width, int height,
                 float *p, float *ip,
                 float *q, float *iq,
                 float *Z_real, float *Z_imag);

// Function to perform zero-padding on an image
void zeroPadImage(const unsigned char *input,
                  unsigned char **output,
                  int originalWidth, int originalHeight, int channels,
                  int paddedWidth, int paddedHeight);

/// Function to apply zero padding on a uint32_t input image and store it in a uint8_t output array.
// Each uint32_t contains 4 uint8_t values that are unpacked and padded with zeros where necessary.
void zeroPadImageFromUint32Pack(uint32_t *input, uint8_t *output,
                                int originalWidth, int originalHeight,
                                int paddedWidth, int paddedHeight);
