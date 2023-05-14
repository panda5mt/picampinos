#include "pico/stdlib.h"
#include "pico_dct8.h"
#include "pico_fft.h"
#define BLOCK_SIZE 8

void pico_dct8(uint8_t* image, float_t *cff, int32_t img_height, int32_t img_width) {
    
    int32_t u, v, start_row, start_col;
    int32_t block_rows = img_height / BLOCK_SIZE ;
    int32_t block_cols = img_width / BLOCK_SIZE ;
    float_t cu, cv, cos_u, cos_v, sum, pixel_value, coeff;
   
    // 8x8ブロックごとに処理
    for (int32_t row = 0; row < block_rows; row++) {
        for (int32_t col = 0; col < block_cols; col++) {

            // 8x8ブロックの先頭位置を計算
            start_row = row * BLOCK_SIZE;
            start_col = col * BLOCK_SIZE;

            // 8x8ブロック内の各係数を計算
            for (u = 0; u < BLOCK_SIZE; u++) {
                for (v = 0; v < BLOCK_SIZE; v++) {
                    // 係数のスケーリング
                    cu = (u == 0) ? 1.0 / sqrt(2.0) : 1.0;
                    cv = (v == 0) ? 1.0 / sqrt(2.0) : 1.0;
                    sum = 0.0;

                    // 8x8ブロック内の各ピクセルに対してDCTを計算
                    for (int32_t x = 0; x < BLOCK_SIZE; x++) {
                        for (int32_t y = 0; y < BLOCK_SIZE; y++) {
                            pixel_value = image[(start_row + y) * img_width + (start_col + x)];
                            cos_u = cos((2 * x + 1) * u * M_PI / (2.0 *BLOCK_SIZE));
                            cos_v = cos((2 * y + 1) * v * M_PI / (2.0 *BLOCK_SIZE));
                            sum += pixel_value * cos_u * cos_v;
                        }
                    }             
                    coeff = (0.25 * cu * cv * sum);
                    // 係数を格納する処理
                    cff[((row * BLOCK_SIZE + v) * img_width) + col * BLOCK_SIZE + u] = coeff;                                
                }
            }
        }
    }
}

