/*		basstat.c		*/
#include <stdio.h>
#include "sslib.h"

void sorti1(int a[], int n)
{
	int i, j, k, l, r, s, x, w, st1[32], st2[32];

	if(n <= 1)
	{
		fprintf(stderr, "Error : n <= 1  in sorti1()\n");
		return;
	}
	s = 0;
	st1[0] = 0;
	st2[0] = n - 1;
	do
	{
		l = st1[s];
		r = st2[s];
		s--;
		if(r - l < 11)
		{
			i = l;
			while(i < r)
			{
				k = i++;
				j = i;
				while(*(a + k) > *(a + j))
				{
					w = *(a + k);
					*(a + k) = *(a + j);
					*(a + j) = w;
					if(j <= l + 1)	break;
					j = k--;
				}
			}
		}
		else
		{
			for(;;)
			{
				i = l;
				j = r;
				x = *(a + (int)((l + r) / 2));
				for(;;)
				{
					while(x > *(a + i))	i++;
					while(*(a + j) > x)	j--;
					if(i > j)	break;
					w = *(a + i);
					*(a + i++) = *(a + j);
					*(a + j--) = w;
					if(i > j)	break;
				}
				if(j - l < r - i)
				{
					if(i < r)
					{
						s++;
						st1[s] = i;
						st2[s] = r;
					}
					r = j;
				}
				else
				{
					if(l < j)
					{
						s++;
						st1[s] = l;
						st2[s] = j;
					}
					l = i;
				}
				if(l >= r)	break;
			}
		}
	} while(s >= 0);
	return;
}

void sorti2(int a[], int n)
{
	int *p, *q, w;

	if(n <= 1)
	{
		fprintf(stderr, "Error : n <= 1  in sorti2()\n");
		return;
	}
	sorti1(a, n);
	p = a;
	q = a + n - 1;
	while(p < q)
	{
		w = *p;
		*p++ = *q;
		*q-- = w;
	}
	return;
}

void sortii1(int a[], int n, int jun[])
{
	int i, j, k, l, *p, r, s, w, x, st1[32], st2[32];

	if(n <= 1)
	{
		fprintf(stderr, "Error : n <= 1  in sortii1()\n");
		return;
	}
	for(i = 0, p = jun; i < n; i++)	*p++ = i;
	s = 0;
	st1[0] = 0;
	st2[0] = n - 1;
	do
	{
		l = st1[s];
		r = st2[s];
		s--;
		if(r - l < 11)
		{
			i = l;
			while(i < r)
			{
				k = i++;
				j = i;
				while(*(a + *(jun + k)) > *(a + *(jun + j)))
				{
					w = *(jun + k);
					*(jun + k) = *(jun + j);
					*(jun + j) = w;
					if (j <= l + 1)	break;
					j = k--;
				}
			}
		}
		else
		{
			for(;;)
			{
				i = l;
				j = r;
				x = *(a + *(jun + (int)((l + r) / 2)));
				for(;;)
				{
					while(x > *(a + *(jun + i)))	i++;
					while(*(a + *(jun + j)) > x)	j--;
					if(i > j)	break;
					w = *(jun + i);
					*(jun + i++) = *(jun + j);
					*(jun + j--) = w;
					if(i > j)	break;
				}
				if(j - l < r - i)
				{
					if(i < r)
					{
						s++;
						st1[s] = i;
						st2[s] = r;
					}
					r = j;
				}
				else
				{
					if(l < j)
					{
						s++;
						st1[s] = l;
						st2[s] = j;
					}
					l = i;
				}
				if(l >= r)	break;
			}
		}
	} while(s >= 0);
	return;
}

void sortii2(int a[], int n, int jun[])
{
	int *p, *q, w;

	if(n <= 1)
	{
		fprintf(stderr, "Error : n <= 1  in sortii2()\n");
		return;
	}
	sortii1(a, n, jun);
	p = jun;
	q = jun + n - 1;
	while(p < q)
	{
		w = *p;
		*p++ = *q;
		*q-- = w;
	}
	return;
}

void sortd1(double a[], int n)
{
	int i, j, k, l, r, s, st1[32], st2[32];
	double x, w;

	if(n <= 1)
	{
		fprintf(stderr, "Error : n <= 1  in sortd1()\n");
		return;
	}
	s = 0;
	st1[0] = 0;
	st2[0] = n - 1;
	do
	{
		l = st1[s];
		r = st2[s];
		s--;
		if(r - l < 11)
		{
			i = l;
			while(i < r)
			{
				k = i++;
				j = i;
				while(*(a + k) > *(a + j))
				{
					w = *(a + k);
					*(a + k) = *(a + j);
					*(a + j) = w;
					if(j <= l + 1)	break;
					j = k--;
				}
			}
		}
		else
		{
			for(;;)
			{
				i = l;
				j = r;
				x = *(a + (int)((l + r) / 2));
				for(;;)
				{
					while(x > *(a + i))	i++;
					while(*(a + j) > x)	j--;
					if(i > j)	break;
					w = *(a + i);
					*(a + i++) = *(a + j);
					*(a + j--) = w;
					if(i > j)	break;
				}
				if(j - l < r - i)
				{
					if(i < r)
					{
						s++;
						st1[s] = i;
						st2[s] = r;
					}
					r = j;
				}
				else
				{
					if(l < j)
					{
						s++;
						st1[s] = l;
						st2[s] = j;
					}
					l = i;
				}
				if(l >= r)	break;
			}
		}
	} while(s >= 0);
	return;
}

void sortd2(double a[], int n)
{
	double *p, *q, w;

	if(n <= 1)
	{
		fprintf(stderr, "Error : n <= 1  in sortd2()\n");
		return;
	}
	sortd1(a, n);
	p = a;
	q = a + n - 1;
	while(p < q)
	{
		w = *p;
		*p++ = *q;
		*q-- = w;
	}
	return;
}

void sortdi1(double a[], int n, int jun[])
{
	int i, j, k, l, *p, r, s, w, st1[32], st2[32];
	double x;

	if(n <= 1)
	{
		fprintf(stderr, "Error : n <= 1  in sortdi1()\n");
		return;
	}
	for(i = 0, p = jun; i < n; i++)	*p++ = i;
	s = 0;
	st1[0] = 0;
	st2[0] = n - 1;
	do
	{
		l = st1[s];
		r = st2[s];
		s--;
		if(r - l < 11)
		{
			i = l;
			while(i < r)
			{
				k = i++;
				j = i;
				while(*(a + *(jun + k)) > *(a + *(jun + j)))
				{
					w = *(jun + k);
					*(jun + k) = *(jun + j);
					*(jun + j) = w;
					if(j <= l + 1)	break;
					j = k--;
				}
			}
		}
		else
		{
			for(;;)
			{
				i = l;
				j = r;
				x = *(a + *(jun + (int)((l + r) / 2)));
				for(;;)
				{
					while(x > *(a + *(jun + i)))	i++;
					while(*(a + *(jun + j)) > x)	j--;
					if(i > j)	break;
					w = *(jun + i);
					*(jun + i++) = *(jun + j);
					*(jun + j--) = w;
					if(i > j)	break;
				}
				if(j - l < r - i)
				{
					if(i < r)
					{
						s++;
						st1[s] = i;
						st2[s] = r;
					}
					r = j;
				}
				else
				{
					if(l < j)
					{
						s++;
						st1[s] = l;
						st2[s] = j;
					}
					l = i;
				}
				if(l >= r)	break;
			}
		}
	} while(s >= 0);
	return;
}

void sortdi2(double a[], int n, int jun[])
{
	int *p, *q, w;

	if(n <= 1)
	{
		fprintf(stderr, "Error : n <= 1  in sortdi2()\n");
		return;
	}
	sortdi1(a, n, jun);
	p = jun;
	q = jun + n - 1;
	while(p < q)
	{
		w = *p;
		*p++ = *q;
		*q-- = w;
	}
	return;
}

int stat(double a[], int n, double *min, double *max, double *amp, double *sum, double *sum2, double *ave, double *gmean, double *hmean, double *amad, double *s, double *v, double *st, double *uv, double *squv, double *cv, double *sk, double *ku)
{
	double dn, *p, sum3, sum4, w, w3;
	int flag, i;

	if(n < 2)
	{
		fprintf(stderr, "Error : n < 2  in stat()\n");
		return -1;
	}
	dn = (double)n;
	*min = *max = a[0];
	*sum = *sum2 = sum3 = sum4 = 0.;
	for(i = 0, p = a; i < n; i++, p++)
	{
		if(*min > *p)	*min = *p;
		else if(*max < *p)	*max = *p;
		*sum += *p;
		*sum2 += (w = *p * *p);
	}
	*amp = (*min + *max) / 2.;
	*ave = *sum / dn;
	for(i = 0, *amad = 0., p = a; i < n; i++, p++)
	{
		*amad += fabs(w = *p - *ave);
		sum3 += (w3 = w * w * w);
		sum4 += (w3 * w);
	}
	*amad /= dn;
	*v = *sum2 / dn - *ave * *ave;
	*s = *v * dn;
	*st = sqrt(*v);
	*uv = *s / (dn - 1.);
	*squv = sqrt(*uv);
	*cv = sqrt(*v) / *ave * 100.;
	*sk = sum3 / dn / *st / *st / *st;
	*ku = sum4 / dn / *v / *v;
	for(i = 0, p = a, flag = 0, *gmean = *hmean = 0.; i < n; i++, p++)
	{
		if(*p == 0.)
		{
			fprintf(stderr, "Warning : (data) = 0  in geometric & harmonic mean calculation of stat()\n");
			flag = 1;
			break;
		}
		if(*p < 0.)
		{
			fprintf(stderr, "Warning : (data) < 0  in geometric mean calculation of stat()\n");
			flag = 1;
			break;
		}
		*gmean += log(*p);
		*hmean += 1. / *p;
	}
	if(flag)
	{
		*gmean = *hmean = 0.;
		return 1;
	}
	*gmean = exp(*gmean / dn);
	*hmean = dn / *hmean;
	return 0;
}

void swapi(int *a, int *b)
{
	int w;

	w = *a;
	*a = *b;
	*b = w;
	return;
}

void swapd(double *a, double *b)
{
	double w;

	w = *a;
	*a = *b;
	*b = w;
	return;
}

void swapc(char *a, char *b)
{
	char w;

	w = *a;
	*a = *b;
	*b = w;
	return;
}

void swapx(Complex *a, Complex *b)
{
	Complex w;

	w = *a;
	*a = *b;
	*b = w;
	return;
}