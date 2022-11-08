#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*
高速フーリエ変換
int32_t n ：データ数（２のべき乗）
int32_t is_inverse ：順変換:-1, 逆変換:1
float_t *ar ：データ配列（実部；要素数 n)
float_t *ai ：データ配列（虚部；要素数 n）
*/
int32_t my_fft(int32_t n, int32_t is_inverse, float_t* ar, float_t* ai);
int32_t my_fft2(int32_t n, int32_t nmax, int32_t is_inverse, float_t* ar, float_t* ai, float_t* wr, float_t* wi);
