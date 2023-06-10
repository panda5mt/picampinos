#include "pico/stdlib.h"
#include "pico_leq.h"

// Gauss elimination 
void pico_ge(float_t A[G_NUM][G_NUM], float_t b[G_NUM], float_t x[G_NUM]) {
    int i, j, k;
    float_t temp;

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
void pico_gs(float_t A[G_NUM][G_NUM], float_t b[G_NUM], float_t x[G_NUM]) {
    int i, j, k;
    float_t old_x[G_NUM];
    
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
            float_t sum1 = 0.0;
            for (j = 0; j < i; j++) {
                sum1 += A[i][j] * x[j];
            }
            float_t sum2 = 0.0;
            for (j = i + 1; j < G_NUM; j++) {
                sum2 += A[i][j] * old_x[j];
            }
            x[i] = (b[i] - sum1 - sum2) / A[i][i];
        }

        // 収束判定
        float_t error = 0.0;
        for (i = 0; i < G_NUM; i++) {
            error += fabs(x[i] - old_x[i]);
        }
        if (error < EPSILON) {
            break;
        }
    }
}

// Skyline 
void forward_elimination(float_t* A, float_t* b, int* index, int n) {
    for (int k = 0; k < n-1; k++) {
        for (int i = k+1; i < n; i++) {
            float_t factor = A[index[i]+k] / A[index[k]+k];
            for (int j = k+1; j < n; j++) {
                A[index[i]+j] -= factor * A[index[k]+j];
            }
            b[i] -= factor * b[k];
        }
    }
}

void backward_substitution(float_t* A, float_t* b, int* index, int n, float_t* x) {
    for (int i = n-1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i+1; j < n; j++) {
            x[i] -= A[index[i]+j] * x[j];
        }
        x[i] /= A[index[i]+i];
    }
}

void skyline_solve(float_t* A, float_t* b, int n, float_t* x) {
    // 前処理: 非ゼロ要素の開始位置を示すインデクス配列を計算
    int* index = malloc(n * sizeof(int));
    int nz = 0;
    for (int i = 0; i < n; i++) {
        index[i] = nz;
        for (int j = 0; j < n; j++) {
            if (A[i*n+j] != 0.0) {
                nz++;
            }
        }
    }

    // 前進消去
    forward_elimination(A, b, index, n);

    // 後退代入
    backward_substitution(A, b, index, n, x);

    free(index);
}
