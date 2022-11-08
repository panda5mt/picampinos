#include "my_fft.h"


const float_t __mpi = 3.141592;

int32_t my_fft(int32_t n, int32_t flg, float_t* ar, float_t* ai)
{
    long m, mh, i, j, k, irev;
    float_t wr, wi, xr, xi;
    float_t theta;

    theta = flg * 2 * __mpi / n;

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

    if( flg == -1 ){
        for(i=0; i<n; i++){
            ar[i] /= n;
            ai[i] /= n;
        }
    }
    return 0;
}