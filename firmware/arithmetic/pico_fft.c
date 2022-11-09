#include "pico_fft.h"


const float_t __mpi = 3.141592;

int32_t _fft(int32_t n, int32_t is_inverse, float_t* ar, float_t* ai)
{
    long m, mh, i, j, k, irev;
    float_t wr, wi, xr, xi;
    float_t theta;

    theta = is_inverse * 2 * __mpi / n;

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
            wr = cos(theta * irev);
            wi = sin(theta * irev);
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
	int i, pp, iter, j, k;
	

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