#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void estimate_lightsource_and_normal(int width, int height,
                                     unsigned char *img_gray, float *p, float *q,
                                     float *L, float *absL);

int32_t fcmethod(int width, int height,
                 float *p, float *ip,
                 float *q, float *iq,
                 float *Z_real, float *Z_imag);

// When 'yuv_to_mono' is true, this function processes a uint32_t input array
// where only the 1st and 3rd bytes of each uint32_t are valid.
// The 2nd and 4th bytes are ignored. The output array is padded with zeros where necessary.
// The original image is stored as a uint32_t array, while the padded output is a uint8_t array.
// Each pixel in the output uses 2 bytes (the 1st and 3rd valid bytes from each uint32_t in the input).
void zeroPadImage(const void *input,
                  unsigned char **output,
                  int originalWidth, int originalHeight, int channels,
                  int paddedWidth, int paddedHeight,
                  bool yuv_to_mono);
