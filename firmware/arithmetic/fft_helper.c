#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fft_helper.h"
// グローバル変数（引数を共有する場合）
// typedef struct
// {
//     int hei;
//     int wid;
//     float *q;
//     int *ip;
//     float *w;
// } TaskArgs;

TaskArgs taskArgs;
TaskHandle_t FFTTaskHandle = NULL;
TaskHandle_t getCurrentHandle;

void send_notify_to_task(void)
{
    // 処理タスクへ通知を送信

    printf("Triggering processing task...\n");
    xTaskNotify(FFTTaskHandle, 0, eNoAction);
    return;
}

void recv_task_end_flag()
{
    // 処理終了フラグを確認
    uint32_t ulNotificationValue = 0; // 初期化
    if (xTaskNotifyWait(0, 0xFFFFFFFF, &ulNotificationValue, portMAX_DELAY) == pdTRUE)
    {
        if (ulNotificationValue & 0x1)
        {
            printf("Processing completed. Waiting for next trigger...\n");
        }
    }
}

void vProcessingFFTTask(void *pvParameters)
{
    uint32_t ulNotificationValue;
    printf("FFTTask - Running on Core: %d\n", get_core_num()); // 現在のコア番号を表示
    for (;;)
    {
        // トリガーを待機 (通知を待機)
        if (xTaskNotifyWait(0, 0xFFFFFFFF, &ulNotificationValue, portMAX_DELAY) == pdTRUE)
        {
            // 引数の取得
            int height = taskArgs.hei;
            int width = taskArgs.wid;
            float **q = taskArgs.q;
            int *ip = taskArgs.ip;
            float *w = taskArgs.w;

            rdft2d(height, width, 1, q, ip, w);

            // トリガータスクへ通知を送信
            xTaskNotify(imageHandle, 0x1, eSetValueWithoutOverwrite);
        }
    }
}