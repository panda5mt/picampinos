#include "int_fft.h"

Complex int_lift(Complex x, Complex w) {

    int32_t c, s; 
    c = w.r; s = w.i; // both c,s are real number
    int32_t ar,ai; 
    ar = x.r; ai = x.i;

    if (( -1.0e-10 < s ) && ( s < 1.0e-10 )) {
        // do nothing
    }else if(c>= 0.0) {
        ar = ar + ai * (c - 1)/s;
        ai = ai + ar * s;
        ar = ar + ar * (c - 1)/s;
    }else{
        ar = ar + ai * (c + 1)/s;
        ai = ai + ar * (-s);
        ar = ar + ai * (c + 1)/s;
        ar = -ar ;
        ai = -ai ;        
    }
    return tocomplex(ar, ai);
}

Complex int_fft(Complex *aa) {


}
// misc
Complex tocomplex(int32_t x, int32_t y)
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

Complex cmul1(Complex a, int32_t k)
{
	return tocomplex(a.r * k, a.i * k);
}

Complex cdiv(Complex a, Complex b)
{
	int32_t w;

	if(ceq(a, b))	return tocomplex(1., 0.);
	w = b.r * b.r + b.i * b.i;
	if(w == 0)
	{
		//fprintf(stderr, "Error : Divide by 0 in cdiv()\n");
		return tocomplex(0., 0.);
	}
	return tocomplex((a.r * b.r + a.i * b.i) / w, (a.i * b.r - a.r * b.i) / w);
}

Complex cdiv1(Complex a, int32_t k)
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
	int32_t x;

	x = exp(a.r);
	return tocomplex(x * cos(a.i), x * sin(a.i));
}

int32_t c_abs_sqrt(Complex a)
{
	return sqrt(a.r * a.r + a.i * a.i);
}

int32_t c_arg(Complex a)
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