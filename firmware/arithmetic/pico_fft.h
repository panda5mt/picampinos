#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// internal APIs
// int32_t _fft(int32_t n, int32_t is_inverse, float_t* ar, float_t* ai);
// int32_t _fft2(int32_t n, int32_t nmax, int32_t is_inverse, float_t* ar, float_t* ai, float_t* wr, float_t* wi);

// wrapper
void pico_fft(int32_t n, float_t* ar, float_t* ai) ;
void pico_ifft(int32_t n, float_t* ar, float_t* ai) ;

/// @brief 2D FFT
/// @param n     row size of array
/// @param nmax  columb size of array (may be better to have n equal to nmax)
/// @param ar    array of real number (ar must be 1D array)
/// @param ai    array of imaginal number (ai must be 1D array)
/// @param wr    working resistor of real (must be declared with length n. ex:wr[n])
/// @param wi    working register of imag (must be declared with length n. ex:wi[n])
void pico_fft2(int32_t n, int32_t nmax, float_t* ar, float_t* ai, float_t* wr, float_t* wi) ;

/// @brief 2D Inverse FFT
/// @param n     row size of array
/// @param nmax  columb size of array (may be better to have n equal to nmax)
/// @param ar    array of real number (ar must be 1D array)
/// @param ai    array of imaginal number (ai must be 1D array)
/// @param wr    working resistor of real (must be declared with length n. ex:wr[n])
/// @param wi    working register of imag (must be declared with length n. ex:wi[n])
void pico_ifft2(int32_t n, int32_t nmax, float_t* ar, float_t* ai, float_t* wr, float_t* wi) ;
