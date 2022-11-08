#include "int_fft.h"

Complex int_lift(Complex *x, Complex w) {

    float_t     c, s; 
    c = w.r; s = w.i; // both c,s are real number
    Complex     *a ; 
    a->r = x->r; a->i = x->i;
    Complex  a_unit;

    if (( -1.0e-10 < s ) && ( s < 1.0e-10 )) {
        // do nothing
    }else if(c>= 0.0) {
        a_unit =  cmul1(a[0] ,(c - 1)/s);

    }else{

    }
}

Complex int_fft(Complex *aa) {


}
// misc
Complex tocomplex(float_t x, float_t y)
{
	Complex a;

	a.r = x;
	a.i = y;
	return a;
}

Complex cadd(Complex a, Complex b)
{
	return tocomplex(a.r + b.r, a.i + b.i);
}

Complex csub(Complex a, Complex b)
{
	return tocomplex(a.r - b.r, a.i - b.i);
}

Complex cmul(Complex a, Complex b)
{
	return tocomplex(a.r * b.r - a.i * b.i, a.r * b.i + a.i * b.r);
}

Complex cmul1(Complex a, float_t k)
{
	return tocomplex(a.r * k, a.i * k);
}

Complex cdiv(Complex a, Complex b)
{
	float_t w;

	if(ceq(a, b))	return tocomplex(1., 0.);
	w = b.r * b.r + b.i * b.i;
	if(w == 0)
	{
		//fprintf(stderr, "Error : Divide by 0 in cdiv()\n");
		return tocomplex(0., 0.);
	}
	return tocomplex((a.r * b.r + a.i * b.i) / w, (a.i * b.r - a.r * b.i) / w);
}

Complex cdiv1(Complex a, float_t k)
{
	if(k == 0.)
	{
		//fprintf(stderr, "Error : Divide by 0 in cdiv1()\n");
		return tocomplex(0., 0.);
	}
	return tocomplex(a.r / k, a.i / k);
}

Complex c_ln(Complex a)
{
	return tocomplex(log(c_abs_sqrt(a)), c_arg(a));
}

Complex c_log10(Complex a)
{
	return cmul(tocomplex(M_LOG10E, 0.), c_ln(a));
}

Complex c_exp(Complex a)
{
	float_t x;

	x = exp(a.r);
	return tocomplex(x * cos(a.i), x * sin(a.i));
}

float_t c_abs_sqrt(Complex a)
{
	return sqrt(a.r * a.r + a.i * a.i);
}

float_t c_arg(Complex a)
{
	if(c_abs_sqrt(a) == 0.)	return 0.;
	return atan2(a.i, a.r);
}

bool ceq(Complex a, Complex b)
{
	if(a.r == b.r && a.i == b.i)	return 1;
	return 0;
}

bool cne(Complex a, Complex b)
{
	if(a.r != b.r || a.i != b.i)	return 1;
	return 0;
}