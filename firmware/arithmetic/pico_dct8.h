#include "pico/stdlib.h"
#include "pico/divider.h"
#include "math.h"

void make_dct_table(void) ;
void pico_dct8(uint8_t* image, float_t *cff, int32_t img_height, int32_t img_width) ;
void pico_idct8(float_t* image, int32_t *cff, int32_t img_height, int32_t img_width) ;
 