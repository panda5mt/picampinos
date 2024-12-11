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

// RGB565のGreen成分を8bitにスケールアップ
uint8_t extract_green_from_rgb565(uint16_t rgb565)
{
    // 緑の部分はビット[5:10] (6ビット)
    uint8_t green = (rgb565 >> 5) & 0x3F; // 緑のビット抽出（6ビット）
    // 6ビットから8ビットにスケールアップ（<< 2）
    return (green << 2) | (green >> 4); // スケールアップして8ビット化
}

// uint32_t配列からgreenデータを抽出し、uint8_t配列に格納
void extract_green_from_uint32_array(const uint32_t *input, uint8_t *output, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        // 32bitのデータから2つの16bit RGB565を取り出す
        uint16_t rgb1 = input[i] & 0xFFFF;         // 下位16bit
        uint16_t rgb2 = (input[i] >> 16) & 0xFFFF; // 上位16bit

        // 緑の成分を抽出し、出力配列に格納
        output[2 * i] = extract_green_from_rgb565(rgb1);
        output[2 * i + 1] = extract_green_from_rgb565(rgb2);
    }
}

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