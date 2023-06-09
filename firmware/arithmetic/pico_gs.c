#include "pico/stdlib.h"
#include "pico_gs.h"

// Gauss-Seidel
void pico_gs(float_t a[G_NUM][G_NUM], float_t b[G_NUM], float_t x[G_NUM]) {
    int i, j;
    float_t old_x[G_NUM], diff;

    // 初期解を0に設定
    for(i = 0; i < G_NUM; i++) {
        x[i] = 0.0;
    }

    do {
        for(i = 0; i < G_NUM; i++) {
            old_x[i] = x[i];  // 前回の解を保存
        }

        for(i = 0; i < G_NUM; i++) {
            float_t sum = 0.0;
            for(j = 0; j < G_NUM; j++) {
                if(i != j) {
                    sum += a[i][j] * x[j];
                }
            }
            x[i] = (b[i] - sum) / a[i][i];  // 新しい解を計算
        }

        diff = 0.0;
        for(i = 0; i < G_NUM; i++) {
            diff += fabs(x[i] - old_x[i]);  // 解の変化量を計算
        }

    } while(diff > EPSILON);  // 解が収束するまで反復
}