#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "sfe_pico.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "pico/async_context_freertos.h"

int *alloc_1d_int(int n1);
void free_1d_int(int *i);
float *alloc_1d_float(int n1);
float **alloc_2d_float(int n1, int n2);
void free_2d_float(float **dd);
void cdft2d(int n1, int n2, int isgn, float **a, int *ip, float *w);
void rdft2d(int n1, int n2, int isgn, float **a, int *ip, float *w);