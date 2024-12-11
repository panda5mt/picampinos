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

// FFT task
void vProcessingFFTTask(void *pvParameters);

void send_notify_to_task(void);

void recv_task_end_flag(void);
