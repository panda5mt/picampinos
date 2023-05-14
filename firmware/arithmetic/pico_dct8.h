#include "pico/stdlib.h"
#include "pico/divider.h"
#include "math.h"

void pico_dct8(uint8_t* image, float *cff, int32_t img_height, int32_t img_width);