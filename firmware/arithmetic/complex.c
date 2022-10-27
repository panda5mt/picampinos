/*		complex.c		*/
#include <stdio.h>
#include "sslib.h"

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

Complex cmul1(Complex a, double k)
{
	return tocomplex(a.r * k, a.i * k);
}

Complex cdiv(Complex a, Complex b)
{
	double w;

	if(ceq(a, b))	return tocomplex(1., 0.);
	w = b.r * b.r + b.i * b.i;
	if(w == 0)
	{
		fprintf(stderr, "Error : Divide by 0 in cdiv()\n");
		return tocomplex(0., 0.);
	}
	return tocomplex((a.r * b.r + a.i * b.i) / w, (a.i * b.r - a.r * b.i) / w);
}

Complex cdiv1(Complex a, double k)
{
	if(k == 0.)
	{
		fprintf(stderr, "Error : Divide by 0 in cdiv1()\n");
		return tocomplex(0., 0.);
	}
	return tocomplex(a.r / k, a.i / k);
}

Complex cln(Complex a)
{
	return tocomplex(log(cabslt(a)), carg(a));
}

Complex clog10(Complex a)
{
	return cmul(tocomplex(M_LOG10E, 0.), cln(a));
}

Complex cexp(Complex a)
{
	double x;

	x = exp(a.r);
	return tocomplex(x * cos(a.i), x * sin(a.i));
}

Complex cpow(Complex a, Complex b)
{
	return cexp(cmul(cln(a), b));
}

Complex cpowd(Complex a, double n)
{
	double w, t;

	t = carg(a) * n;
	w = pow(cabslt(a), n);
	return tocomplex(w * cos(t), w * sin(t));
}

Complex csin(Complex a)
{
	double ar;

	if(fabs(a.r) > 2. * M_PI)	ar = (int)(a.r / 2. / M_PI) * 2. * M_PI;
	else						ar = a.r;
	return tocomplex(sin(ar) * cosh(a.i), cos(ar) * sinh(a.i));
}

Complex ccos(Complex a)
{
	double ar;

	if(fabs(a.r) > 2. * M_PI)	ar = (int)(a.r / 2. / M_PI) * 2. * M_PI;
	else						ar = a.r;
	return tocomplex(cos(ar) * cosh(a.i), - sin(ar) * sinh(a.i));
}

Complex ctan(Complex a)
{
	Complex x;

	if(fabs(a.r) > 2. * M_PI)	a.r = (int)(a.r / 2. / M_PI) * 2. * M_PI;
	x = tocomplex(cos(a.r) * cosh(a.i), - sin(a.r) * sinh(a.i));
	if(x.r == 0. && x.i == 0.)
	{
		fprintf(stderr, "Error : Divide by 0. in ctan()\n");
		return tocomplex(0., 0.);
	}
	return cdiv(csin(a), x);
}

Complex csqrt(Complex a)
{
	double t, z;

	z = pow(cabslt(a), 0.5);
	t = carg(a) / 2.;
	return tocomplex(z * cos(t), z * sin(t));
}

Complex tocomplex(double x, double y)
{
	Complex a;

	a.r = x;
	a.i = y;
	return a;
}

Complex conj(Complex a)
{
	return tocomplex(a.r, - a.i);
}

Complex cfunc(Complex *a, int n, Complex x)
{
	Complex *p, y;
	int i;

	p = a;
	y = *p++;
	for(i = 1; i <= n; i++)	y = cadd(cmul(y, x), *p++);
	return y;
}

double cabslt(Complex a)
{
	return sqrt(a.r * a.r + a.i * a.i);
}

double carg(Complex a)
{
	if(cabslt(a) == 0.)	return 0.;
	return atan2(a.i, a.r);
}

int ceq(Complex a, Complex b)
{
	if(a.r == b.r && a.i == b.i)	return 1;
	return 0;
}

int cne(Complex a, Complex b)
{
	if(a.r != b.r || a.i != b.i)	return 1;
	return 0;
}