#include "pico/stdlib.h"
#include "pico_gauss_elim.h"

void gauss_elimination(float A[GAUSS_ELIM_N][GAUSS_ELIM_N+1]) {
    for (int i=0; i<GAUSS_ELIM_N; i++) {
        // Partial Pivoting
        for (int k=i+1; k<GAUSS_ELIM_N; k++)
            if (fabsf(A[i][i]) < fabsf(A[k][i]))
                for (int j=0; j<=GAUSS_ELIM_N; j++) {
                    float temp = A[i][j];
                    A[i][j] = A[k][j];
                    A[k][j] = temp;
                }

        // Transform the matrix to reduced row echelon form
        for (int k=i+1; k<GAUSS_ELIM_N; k++) {
            float factor = A[k][i]/A[i][i];
            for (int j=0; j<=GAUSS_ELIM_N; j++)
                A[k][j] -= factor * A[i][j];
        }
    }
}

void back_substitution(float A[GAUSS_ELIM_N][GAUSS_ELIM_N+1], float x[GAUSS_ELIM_N]) {
    x[GAUSS_ELIM_N-1] = A[GAUSS_ELIM_N-1][GAUSS_ELIM_N] / A[GAUSS_ELIM_N-1][GAUSS_ELIM_N-1];

    for (int i=GAUSS_ELIM_N-2; i>=0; i--) {
        x[i] = A[i][GAUSS_ELIM_N];
        for (int j=i+1; j<GAUSS_ELIM_N; j++)
            x[i] = x[i] - A[i][j]*x[j];
        x[i] = x[i] / A[i][i];
    }
}
