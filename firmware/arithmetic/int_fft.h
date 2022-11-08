#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"

typedef struct
{
    float_t r;    // real
    float_t i;    // imag
} Complex;

// forward
Complex int_fft(Complex *aa);
Complex int_lift(Complex *x, Complex w) ;

// inverse
Complex int_ifft(Complex *aa);
Complex int_ilift(Complex x, Complex w) ;

// misc
Complex tocomplex(float_t x, float_t y) ;
Complex cadd(Complex a, Complex b) ;
Complex csub(Complex a, Complex b) ;
Complex cmul1(Complex a, float_t k);
Complex cmul(Complex a, Complex b);
Complex cdiv(Complex a, Complex b);
Complex cdiv1(Complex a, float_t k);
Complex c_ln(Complex a);
Complex c_log10(Complex a) ;
Complex c_exp(Complex a) ;
float_t c_abs_sqrt(Complex a) ;
float_t c_arg(Complex a) ;
bool ceq(Complex a, Complex b);
bool cne(Complex a, Complex b);