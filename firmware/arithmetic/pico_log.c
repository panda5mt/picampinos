#include "pico/stdlib.h"
#include "pico/divider.h"
#include "pico_log.h"


#include <stdint.h>
double log_approx(uint32_t x, uint32_t base) {
    // base:2 ,10
    
    // calc log2(x)
    int n = 0;
    if (x >= 0x10000) { x >>= 16; n += 16; }
    if (x >= 0x100) { x >>= 8; n += 8; }
    if (x >= 0x10) { x >>= 4; n += 4; }
    if (x >= 0x4) { x >>= 2; n += 2; }
    if (x >= 0x2) { n += 1; }

    // get upper n-bits of x
    uint32_t r = (x >> (n - 1)) & 0x3;
    uint32_t f = (x << (33 - n)) >> 24;

    // get log2(f)
    int k = 0;
    while (f >= 0x2) {
        f >>= 1;
        k++;
    }

    if(10 == base)
        return (double)(n - 1 + k);
    else
        return (double)(n - 1 + k) * 0.30102999566398119521373889472449; // log2(10) ≈ 0.30102999566398119521373889472449

}
