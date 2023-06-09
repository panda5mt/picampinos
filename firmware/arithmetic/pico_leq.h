#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// solver for linear equation
#define G_NUM 3
#define EPSILON 4

// Gauss-Seidel
void pico_gs(int32_t A[G_NUM][G_NUM], int32_t b[G_NUM], int32_t x[G_NUM]) ;

// Skyline method
void forward_elimination(int32_t* A, int32_t* b, int* index, int n) ;
void backward_substitution(int32_t* A, int32_t* b, int* index, int n, int32_t* x) ;
void skyline_solve(int32_t* A, int32_t* b, int n, int32_t* x) ;