#include "pico/stdlib.h"
#include "pico/divider.h"
#include "math.h"

#define GAUSS_ELIM_N 3  // size of matrix A

void gauss_elimination(float A[GAUSS_ELIM_N][GAUSS_ELIM_N+1]) ;
void back_substitution(float A[GAUSS_ELIM_N][GAUSS_ELIM_N+1], float x[GAUSS_ELIM_N]) ;