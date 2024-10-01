#include <stdio.h>
#include <stdlib.h>
// #include "pico/stdlib.h"

void estimate_lightsource_with_sobel(int width, int height,
                                     unsigned char *img_gray, float *p, float *q,
                                     float *L, float *absL);

// 法線ベクトルを用いて p と q を計算する関数
void estimate_normal(int width, int height,
                     unsigned char *img_gray, float *p, float *q,
                     float *L);

// Function to perform zero-padding on an image
void zeroPadImage(const unsigned char *input, unsigned char **output,
                  int originalWidth, int originalHeight,
                  int channels,
                  int paddedWidth, int paddedHeight);

// // fcmethod関数の実装
int32_t fcmethod(int width, int height,
                 float p[], float ip[],
                 float q[], float iq[],
                 float Z_real[], float Z_imag[]);
