#include "pico/stdlib.h"
#include "pico/divider.h"
#include "pico_log.h"


#include <stdint.h>
double log_approx(uint32_t x, uint32_t base) {
    // base:2 ,10
    
    // log2(x)を求める
    int n = 0;
    if (x >= 0x10000) { x >>= 16; n += 16; }
    if (x >= 0x100) { x >>= 8; n += 8; }
    if (x >= 0x10) { x >>= 4; n += 4; }
    if (x >= 0x4) { x >>= 2; n += 2; }
    if (x >= 0x2) { n += 1; }

    // xの最上位ビットからnビットを取り出す
    uint32_t r = (x >> (n - 1)) & 0x3;
    uint32_t f = (x << (33 - n)) >> 24;

    // log2(f)を求める
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
