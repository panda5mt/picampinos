#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../sparkfun_pico/sfe_pico.h"

float **alloc_2d_float(int n1, int n2);
void free_2d_float(float **dd);
void cdft2d(int n1, int n2, int isgn, float **a, int *ip, float *w);