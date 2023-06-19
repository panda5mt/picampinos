#include "pico/stdlib.h"
#include "pico_leq.h"

// Gauss elimination 
void pico_ge(int32_t A[G_NUM][G_NUM], int32_t b[G_NUM], int32_t x[G_NUM]) {
    int i, j, k;
    int32_t temp;

    // 前進消去
    for (i = 0; i < G_NUM; i++) {
        for (j = i + 1; j < G_NUM; j++) {
            temp = A[j][i] / A[i][i];
            for (k = i; k < G_NUM; k++) {
                A[j][k] -= A[i][k] * temp;
            }
            b[j] -= b[i] * temp;
        }
    }

    // 後退代入
    for (i = G_NUM - 1; i >= 0; i--) {
        temp = b[i];
        for (j = i + 1; j < G_NUM; j++) {
            temp -= A[i][j] * x[j];
        }
        x[i] = temp / A[i][i];
    }
}


// Gauss-Seidel
#define MAX_ITERATION 5  // 最大反復回数
void pico_gs(int32_t A[G_NUM][G_NUM], int32_t b[G_NUM], int32_t x[G_NUM]) {
    int i, j, k;
    int32_t old_x[G_NUM];
    
    // 初期値の設定
    for (i = 0; i < G_NUM; i++) {
        x[i] = 0.0;
    }

    // 反復計算
    for (k = 0; k < MAX_ITERATION; k++) {
        for (i = 0; i < G_NUM; i++) {
            old_x[i] = x[i];
        }
        
        for (i = 0; i < G_NUM; i++) {
            int32_t sum1 = 0.0;
            for (j = 0; j < i; j++) {
                sum1 += A[i][j] * x[j];
            }
            int32_t sum2 = 0.0;
            for (j = i + 1; j < G_NUM; j++) {
                sum2 += A[i][j] * old_x[j];
            }
            x[i] = (b[i] - sum1 - sum2) / A[i][i];
        }

        // 収束判定
        int32_t error = 0.0;
        for (i = 0; i < G_NUM; i++) {
            error += fabs(x[i] - old_x[i]);
        }
        if (error < EPSILON) {
            break;
        }
    }
}
