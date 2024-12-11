#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fft4f2d.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "pico/async_context_freertos.h"

typedef struct
{
    int hei;
    int wid;
    float **q;
    int *ip;
    float *w;
} TaskArgs;

// 他のソースファイルで定義するタスクハンドルの宣言
extern TaskArgs taskArgs;
extern TaskHandle_t FFTTaskHandle;
extern TaskHandle_t imageHandle;

// Function to perform zero-padding on an image
void zeroPadImage(const unsigned char *input,
                  unsigned char *output,
                  int originalWidth, int originalHeight, int channels,
                  int paddedWidth, int paddedHeight);

void zeroPadImageWithBorder(const unsigned char *input,
                            unsigned char *output,
                            int width, int height, int channels,
                            int borderWidth);

// extract green from RGB565 packed data.
// length: input's length. you will need output[2*length]
// (length = sizeof(input[array])/sizeof(input[0]) )
void extract_green_from_uint32_array(const uint32_t *input, uint8_t *output, size_t length);

// RGB565のGreen成分を8bitにスケールアップ
uint8_t extract_green_from_rgb565(uint16_t rgb565);

// FFT task
void vProcessingFFTTask(void *pvParameters);
void send_notify_to_task(void);
void recv_task_end_flag(void);
