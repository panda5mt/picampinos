#include "pico/stdlib.h"
#include "pico/divider.h"
#include "pico_fft.h"
#define SIZE_TBL 1024  /* MAX FFT INPUT SIZE  */

const float_t _PI = M_PI;
const float_t _PI_PI = (M_PI * M_PI);
const float_t _2PI = (2 * M_PI);

float_t cos_table[SIZE_TBL / 2];
float_t sin_table[SIZE_TBL / 2];

int32_t table_ready = false;

void _init_tables() {
    for (int i = 0; i < SIZE_TBL / 2; i++) {
        cos_table[i] = cos(2 * M_PI * i / SIZE_TBL);
        sin_table[i] = sin(2 * M_PI * i / SIZE_TBL);
    }
    table_ready = true;
}

// lifting functions
Lifting _lift(int32_t xr, int32_t xi, float_t c, float_t s) {
    Lifting x;
    x.r = xr;
    x.i = xi;

    if(s == 0.0) {
        return x;
    }
        
    if(s > c){
        if (s > -c) {// (0.25pi, 0.75pi)
            const int32_t t = xr; xr = xi; xi = t;
            xr += (int32_t) (xi*(s-1)/c);
            xi += (int32_t) (xr*c);
            xr += (int32_t) (xi*(s-1)/c);
            xr = -xr;
        }
        else{ // (0.75pi, 1.25pi)
            xi = -xi;
            xr += (int32_t) (xi*(-c-1)/s);
            xi += (int32_t) (xr*s);
            xr += (int32_t) (xi*(-c-1)/s);
            xr = -xr;
        }
    }
    else{
        if (s < -c){ // (-0.75pi, -0.25pi)
            xr += (int32_t) (xi*(-s-1)/c);
            xi += (int32_t) (xr*c);
            xr += (int32_t) (xi*(-s-1)/c);
            const int32_t t = xr; xr = xi; xi = -t;
        }
        else{ // (-0.25pi, 0.25pi)
            xr += (int32_t) (xi*(c-1)/s);
            xi += (int32_t) (xr*s);
            xr += (int32_t) (xi*(c-1)/s);
        }
    }
    x.r = xr;
    x.i = xi;
    return x;
}

Lifting _ilift(int32_t xr, int32_t xi, float_t c, float_t s)
{
    Lifting x;
    x.r = xr;
    x.i = xi;
    if(s == 0.0){
        return x;
    }

    if(s > c){
        if(s > -c){ 
            xr = -xr;
            xr -= (int32_t) (xi*(s-1)/c);
            xi -= (int32_t) (xr*c);
            xr -= (int32_t) (xi*(s-1)/c);
            const int32_t t = xr; xr = xi, xi = t;
        }
        else{ 
            xr = -xr;
            xr -= (int32_t) (xi*(-c-1)/s);
            xi -= (int32_t) (xr*s);
            xr -= (int32_t) (xi*(-c-1)/s);
            xi = -xi;
        }
    }
    else{
        if(s < -c){ 
            const int32_t t = xr; xr = -xi; xi = t;
            xr -= (int32_t) (xi*(-s-1)/c);
            xi -= (int32_t) (xr*c);
            xr -= (int32_t) (xi*(-s-1)/c);
        }
        else{ // (-0.25pi, 0.25pi)
            xr -= (int32_t) (xi*(c-1)/s);
            xi -= (int32_t) (xr*s);
            xr -= (int32_t) (xi*(c-1)/s);
        }
    }
    x.r = xr;
    x.i = xi;
    return x;
}

// integer fft
void _int_fft(int32_t n, int32_t* ar, int32_t* ai)
{   
    Lifting a;
    float_t theta, s1, c1, s3, c3;
    int32_t mq, j1, j2, j3, x0r, x0i, x1r, x1i, x3r, x3i;
    // L shaped butterflies
    for (int32_t m = n; m > 2; m >>= 1) {
        //theta = -2 * _PI / m;
        theta = -2 / m;
        mq = m >> 2;
        for (int32_t i = 0; i < mq; i++) {
            // int index = (int)(theta * i / _2PI * SIZE_TBL) % SIZE_TBL;
            // int index2 = (int)(theta *3 * i / _2PI * SIZE_TBL) % SIZE_TBL;
            int index = (int)(theta * i * SIZE_TBL) % SIZE_TBL;
            int index2 = (int)(theta *3 * i * SIZE_TBL) % SIZE_TBL;
            
            s1 = sin_table[index];  // sin(theta * i)
            c1 = cos_table[index];  // cos(theta * i)
            s3 = sin_table[index2]; // sin(theta * 3 * i)
            c3 = cos_table[index2]; // cos(theta * 3 * i)
            
            for (int32_t k = m; k <= n; k <<= 2) {
                for (int32_t j0 = k - m + i; j0 < n; j0 += 2 * k) {
                    j1 = j0 + mq;
                    j2 = j1 + mq;
                    j3 = j2 + mq;
                    x1r = ar[j0] - ar[j2];
                    x1i = ai[j0] - ai[j2];
                    ar[j0] += ar[j2];
                    ai[j0] += ai[j2];
                    x3r = ar[j1] - ar[j3];
                    x3i = ai[j1] - ai[j3];
                    ar[j1] += ar[j3];
                    ai[j1] += ai[j3];
                    a = _lift(x1r + x3i, x1i - x3r, c1, s1);
                    ar[j2] = a.r;
                    ai[j2] = a.i;
                    a = _lift(x1r - x3i, x1i + x3r, c3, s3);
                    ar[j3] = a.r;
                    ai[j3] = a.i;
                }
            }
        }
    }

    // radix 2 butterflies
    for (int32_t k = 2; k <= n; k <<= 2) {
        for (int32_t j = k - 2; j < n; j += 2 * k) {
            x0r = ar[j] - ar[j + 1];
            x0i = ai[j] - ai[j + 1];
            ar[j] += ar[j + 1];
            ai[j] += ai[j + 1];
            ar[j + 1] = x0r;
            ai[j + 1] = x0i;
        }
    }
    // unscrambler
    for (int32_t i = 0, j = 1; j < n - 1; j++) {
        for (int32_t k = n >> 1; k > (i ^= k); k >>= 1);
        if (j < i) {
            x0r = ar[j];
            x0i = ai[j];
            ar[j] = ar[i];
            ai[j] = ai[i];
            ar[i] = x0r;
            ai[i] = x0i;
        }
    }
}

// integer ifft
void _int_ifft(int32_t n, int32_t* ar, int32_t* ai) {
    Lifting x;
    int32_t mq, j0, j1, j2, j3, x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i;
    int32_t x2r_, x2i_, x3r_, x3i_;
    float_t theta, s1, c1, s3, c3;
    // scrambler
    for (int32_t i = 0, j = 1; j < n - 1; j++) {
        for (int32_t k = n >> 1; k > (i ^= k); k >>= 1);
        if (j < i) {
            x0r = ar[j];
            x0i = ai[j];
            ar[j] = ar[i];
            ai[j] = ai[i];
            ar[i] = x0r;
            ai[i] = x0i;
        }
    }

    // radix 2 butterflies
    for (int32_t k = 2; k <= n; k <<= 2) {
        for (int32_t j = k - 2; j < n; j += 2 * k) {
            x0r = ar[j];
            x0i = ai[j];
            ar[j] = (x0r + ar[j + 1]) / 2;
            ai[j] = (x0i + ai[j + 1]) / 2;
            ar[j + 1] = x0r - ar[j];
            ai[j + 1] = x0i - ai[j];
        }
    }

    // L shaped butterflies
    for (int32_t m = 4; m <= n; m <<= 1) {
        //theta = - 2 *_PI / m;
        theta =  - 2 / m;
        mq = m >> 2;
        for (int32_t i = 0; i < mq; i++) {
            // int index = (int)(theta * i / _2PI * SIZE_TBL) % SIZE_TBL;
            // int index2 = (int)(theta *3 * i / _2PI * SIZE_TBL) % SIZE_TBL;
            int index = (int)(theta * i * SIZE_TBL) % SIZE_TBL;
            int index2 = (int)(theta *3 * i * SIZE_TBL) % SIZE_TBL;
            
            s1 = sin_table[index];  // sin(theta * i)
            c1 = cos_table[index];  // cos(theta * i)
            s3 = sin_table[index2]; // sin(theta * 3 * i)
            c3 = cos_table[index2]; // cos(theta * 3 * i)

            for (int32_t k = m; k <= n; k <<= 2) {
                for (j0 = k - m + i; j0 < n; j0 += 2 * k) {
                    j1 = j0 + mq;
                    j2 = j1 + mq;
                    j3 = j2 + mq;
                    x0r = ar[j0];
                    x0i = ai[j0];
                    x1r = ar[j1];
                    x1i = ai[j1];
                    x = _ilift(ar[j2], ai[j2], c1, s1);
                    x2r = x.r;
                    x2i = x.i;
                    x = _ilift(ar[j3], ai[j3], c3, s3);
                    x3r = x.r;
                    x3i = x.i;
                    x2r_ = (x2r + x3r)/2;
                    x2i_ = (x2i + x3i)/2;
                    x3r_ = -(x2i - x2i_);
                    x3i_ = (x2r - x2r_);
                    ar[j0] = (x0r + x2r_)/2;
                    ai[j0] = (x0i + x2i_)/2;
                    ar[j1] = (x1r + x3r_)/2;
                    ai[j1] = (x1i + x3i_)/2;
                    ar[j2] = (x0r - ar[j0]);
                    ai[j2] = (x0i - ai[j0]);
                    ar[j3] = (x1r - ar[j1]);
                    ai[j3] = (x1i - ai[j1]);
                }
            }
        }
    }
}

// Danielson-Lanczos FFT
int32_t _fft(int n, int is_inverse, float_t* real, float_t* imag) {
    int i, j, m, mmax, istep;
    float_t tempr, tempi;

    // bit-reversal
    j = 0;
    for(i = 0; i < n; i++) {
        if (j > i) {
            // exchange real 
            tempr = real[j];
            real[j] = real[i];
            real[i] = tempr;
            // exchange imag
            tempi = imag[j];
            imag[j] = imag[i];
            imag[i] = tempi;
        }
        m = n / 2;
        while (m >= 1 && j >= m) {
            j -= m;
            m /= 2;
        }
        j += m;
    }

    // Danielson-Lanczos
    mmax = 1;
    while (n > mmax) {
        istep = 2 * mmax;
        for (m = 0; m < mmax; m++) {
            float_t wr = cos_table[m * n / istep];
            float_t wi = (is_inverse) ? -sin_table[m * n / istep] : sin_table[m * n / istep];
            for (i = m; i < n; i += istep) {
                j = i + mmax;
                tempr = wr * real[j] - wi * imag[j];
                tempi = wr * imag[j] + wi * real[j];
                real[j] = real[i] - tempr;
                imag[j] = imag[i] - tempi;
                real[i] += tempr;
                imag[i] += tempi;
            }
        }
        mmax = istep;
    }

    if(is_inverse) {
        // divide by n
        for (i = 0; i < n; i++) {
            real[i] /= n;
            imag[i] /= n;
        }
    }
    return 0;
}

int32_t _fft2(int32_t n, int32_t nmax, int32_t is_inverse, float_t* ar, float_t* ai, float_t* wr, float_t* wi)
{
	int32_t i, pp, iter, j, k;
	

	if(n < 2)
	{
		//fprintf(stderr, "Error : Illegal parameter in fft2()\n");
		return -1;
	}

	for(j = 0; j < n; j++)
	{
		for(i = 0, pp = j ; i < n ; i++, pp += nmax)
		{
            wr[i] = ar[pp];
            wi[i] = ai[pp];

		}
		_fft(n, is_inverse,wr, wi);        
        for(i = 0, pp = j ; i < n ; i++, pp += nmax)
        {
		    ar[pp] = wr[i];
            ai[pp] = wi[i]; 
        }
        
	}

	for(i = k = 0; i < n; i++, k += nmax)
	{
		for(j = 0, pp = k ; j < n ; j++, pp++)
        {
            wr[j] = ar[pp];
            wi[j] = ai[pp];
        }		
        _fft(n, is_inverse, wr, wi);
		for(j = 0, pp = k; j < n ; j++, pp++)
        {
            ar[pp] = wr[j];
            ai[pp] = wi[j];
		}
	}
	return 0;
}

int32_t _int_fft2(int32_t n, int32_t nmax, bool is_inverse, int32_t* ar, int32_t* ai, int32_t* wr, int32_t* wi) {

	int32_t i, pp, iter, j, k;
	if(n < 2)
	{
		//fprintf(stderr, "Error : Illegal parameter in fft2()\n");
		return -1;
	}

	for(j = 0; j < n; j++)
	{
		for(i = 0, pp = j ; i < n ; i++, pp += nmax)
		{
            wr[i] = ar[pp];
            wi[i] = ai[pp];

		}
        if(is_inverse){
		    _int_ifft(n, wr, wi);
        } else {
            _int_fft(n, wr, wi);
        }
        for(i = 0, pp = j ; i < n ; i++, pp += nmax)
        {
		    ar[pp] = wr[i];
            ai[pp] = wi[i]; 
        }
        
	}

	for(i = k = 0; i < n; i++, k += nmax)
	{
		for(j = 0, pp = k ; j < n ; j++, pp++)
        {
            wr[j] = ar[pp];
            wi[j] = ai[pp];
        }
		
        if(is_inverse){
		    _int_ifft(n, wr, wi);
        } else {
            _int_fft(n, wr, wi);
        }
	
		for(j = 0, pp = k; j < n ; j++, pp++)
        {
            ar[pp] = wr[j];
            ai[pp] = wi[j];
		}
	}
	return 0;
}

void pico_fft(int32_t n, float_t* ar, float_t* ai) {
    if(!table_ready)
        _init_tables();
    int32_t is_inverse = 0; // forward FFT
    _fft(n, is_inverse, ar, ai);
}

void pico_ifft(int32_t n, float_t* ar, float_t* ai) {
    if(!table_ready)
        _init_tables();
    int32_t is_inverse = 1; // inverse FFT
    _fft(n, is_inverse, ar, ai);
}

void pico_fft2(int32_t n, int32_t nmax, float_t* ar, float_t* ai, float_t* wr, float_t* wi) {
    int32_t is_inverse = -1; // forward FFT
    _fft2(n, nmax, is_inverse, ar, ai, wr, wi);
}

void pico_ifft2(int32_t n, int32_t nmax, float_t* ar, float_t* ai, float_t* wr, float_t* wi) {
    int32_t is_inverse = 1; // inverse FFT
    _fft2(n, nmax, is_inverse, ar, ai, wr, wi);
}

void pico_int_fft2(int32_t n, int32_t nmax, int32_t* ar, int32_t* ai, int32_t* wr, int32_t* wi) {
    if(!table_ready)
        _init_tables();
    bool is_inverse = false;
    _int_fft2(n,nmax,is_inverse,ar,ai,wr,wi);
}
void pico_int_ifft2(int32_t n, int32_t nmax, int32_t* ar, int32_t* ai, int32_t* wr, int32_t* wi) {
    if(!table_ready)
        _init_tables();
    bool is_inverse = true;
    _int_fft2(n,nmax,is_inverse,ar,ai,wr,wi);
}

