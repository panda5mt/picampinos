#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// solver for linear equation
#define G_NUM 3
#define EPSILON 1

// Gauss elimination
void pico_ge(int32_t A[G_NUM][G_NUM], int32_t b[G_NUM], int32_t x[G_NUM]) ;

// Gauss-Seidel
void pico_gs(int32_t A[G_NUM][G_NUM], int32_t b[G_NUM], int32_t x[G_NUM]) ;
