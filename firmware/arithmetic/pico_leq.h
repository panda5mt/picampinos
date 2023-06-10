#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// solver for linear equation
#define G_NUM 3
#define EPSILON 4

// Gauss elimination
void pico_ge(float_t A[G_NUM][G_NUM], float_t b[G_NUM], float_t x[G_NUM]) ;

// Gauss-Seidel
void pico_gs(float_t A[G_NUM][G_NUM], float_t b[G_NUM], float_t x[G_NUM]) ;

// Skyline method
void forward_elimination(float_t* A, float_t* b, int* index, int n) ;
void backward_substitution(float_t* A, float_t* b, int* index, int n, float_t* x) ;
void skyline_solve(float_t* A, float_t* b, int n, float_t* x) ;