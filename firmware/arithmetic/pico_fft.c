#include "pico/stdlib.h"
#include "pico/divider.h"
#include "pico_fft.h"


const float_t _PI = M_PI;
const float_t _PI_PI = (M_PI * M_PI);
const float_t _2PI = (2 * M_PI);

float_t _sine(float_t x, uint32_t nMAX) {
    x -= (int32_t)(x / _2PI) * _2PI; 
    float_t sum = x;
    float_t t = x;
 
    for(uint32_t n = 1 ; n <= nMAX ; n++) {
        t *= - (x * x) / ((2 * n + 1) * (2 * n));
        sum += t;
    }
 
    return sum;

}

// nMAX = 1 to 3?
float_t _cosine(float_t x, uint32_t nMAX) {
    //x = _check_angle(x) ;  
    x += _PI/2;
    // if(x > _PI)
    // {
    //     x -= _2PI;  
    // } 
    return _sine(x, nMAX);
}

float_t _fastsin(float_t x) {
   return  _sine(x, 1);
}

float_t _fastcos(float_t x) {
   return  _cosine(x, 1);
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
        theta = -2 * _PI / m;
        mq = m >> 2;
        for (int32_t i = 0; i < mq; i++) {
            s1 = _fastsin(theta * i);
            c1 = _fastcos(theta * i);
            s3 = _fastsin(theta * 3 * i);
            c3 = _fastcos(theta * 3 * i);
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
        theta =  - 2 *_PI / m;
        mq = m >> 2;
        for (int32_t i = 0; i < mq; i++) {
            s1 = _fastsin(theta * i);
            c1 = _fastcos(theta * i);
            s3 = _fastsin(theta * 3 * i);
            c3 = _fastcos(theta * 3 * i);
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

int32_t _fft(int32_t n, int32_t is_inverse, float_t* ar, float_t* ai)
{
    long m, mh, i, j, k, irev;
    float_t wr, wi, xr, xi;
    float_t theta;

    theta = is_inverse * 2 * _PI / n;

    i = 0;
    for (j = 1; j < n - 1; j++) {
        for (k = n >> 1; k > (i ^= k); k >>= 1);
        if (j < i) {
            xr = ar[j];
            xi = ai[j];
            ar[j] = ar[i];
            ai[j] = ai[i];
            ar[i] = xr;
            ai[i] = xi;
        }
    }
    for (mh = 1; (m = mh << 1) <= n; mh = m) {
        irev = 0;
        for (i = 0; i < n; i += m) {
            wr = _cosine(theta * irev, 5);
            wi = _sine(theta * irev, 5);
            for (k = n >> 2; k > (irev ^= k); k >>= 1);
            for (j = i; j < mh + i; j++) {
                k = j + mh;
                xr = ar[j] - ar[k];
                xi = ai[j] - ai[k];
                ar[j] += ar[k];
                ai[j] += ai[k];
                ar[k] = wr * xr - wi * xi;
                ai[k] = wr * xi + wi * xr;
            }
        }
    }    
    
    if( is_inverse == 1 ){
        for(i=0; i<n; i++){
            ar[i] /= n;
            ai[i] /= n;
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
    int32_t is_inverse = -1; // forward FFT
    _fft(n, is_inverse, ar, ai);
}

void pico_ifft(int32_t n, float_t* ar, float_t* ai) {
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
    bool is_inverse = false;
    _int_fft2(n,nmax,is_inverse,ar,ai,wr,wi);
}
void pico_int_ifft2(int32_t n, int32_t nmax, int32_t* ar, int32_t* ai, int32_t* wr, int32_t* wi) {
    bool is_inverse = true;
    _int_fft2(n,nmax,is_inverse,ar,ai,wr,wi);
}

