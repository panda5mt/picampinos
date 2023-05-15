#include "pico/stdlib.h"
#include "pico_dct8.h"
#include "pico_fft.h"


#define BLOCK_SIZE 8
#define FACTOR 512

int dct_table[8 * 8];

void make_dct_table(void) {
    for(int i = 0; i < BLOCK_SIZE; i++) {
        for(int j = 0; j < BLOCK_SIZE; j++) {
            dct_table[i * BLOCK_SIZE + j] = (int) (cos((2 * i + 1) * j * M_PI / (2 * BLOCK_SIZE)) * FACTOR);
            //printf("%8d",dct_table[i * BLOCK_SIZE + j]);
        }
        printf("\n");
    }
}

void pico_dct8(uint8_t* image, float_t *cff, int32_t img_height, int32_t img_width) {
    
    int32_t x, y, u, v, row, col;
    int32_t block_rows = img_height; 
    int32_t block_cols = img_width;
    float_t cu, cv, sum, pixel_value, coeff;
   
    // 8x8ブロックごとに処理
    for (row = 0; row < block_rows; row+=BLOCK_SIZE) {
        for (col = 0; col < block_cols; col+=BLOCK_SIZE) {

            // 8x8ブロック内の各係数を計算
            for (u = 0; u < BLOCK_SIZE; u++) {
                for (v = 0; v < BLOCK_SIZE; v++) {
                    // 係数のスケーリング
                    cu = (u == 0) ? 1.0 / sqrt(2.0) : 1.0;
                    cv = (v == 0) ? 1.0 / sqrt(2.0) : 1.0;
                    sum = 0.0;

                    // 8x8ブロック内の各ピクセルに対してDCTを計算
                    for (x = 0; x < BLOCK_SIZE; x++) {
                        for (y = 0; y < BLOCK_SIZE; y++) {
                            pixel_value = image[(row + y) * img_width + (col + x)];
                            
                            sum += pixel_value  
                                    * dct_table[x * BLOCK_SIZE + u] 
                                    * dct_table[y * BLOCK_SIZE + v] / FACTOR / FACTOR;
                                ;
                        }
                    }             
                    coeff = (0.25 * cu * cv * sum);
                    // 係数を格納する処理
                    cff[((row + v) * img_width) + col + u] = coeff;                                
                }
            }
        }
    }
}
void pico_idct8(float_t* image, int32_t *cff, int32_t img_height, int32_t img_width) {
    int32_t x, y, u, v, row, col;
    int32_t block_rows = img_height; 
    int32_t block_cols = img_width;
    float_t cu, cv, cos_u, cos_v, sum, pixel_value, coeff;
   
    // 8x8ブロックごとに処理
    for (row = 0; row < block_rows; row+=BLOCK_SIZE) {
        for (col = 0; col < block_cols; col+=BLOCK_SIZE) {
            for (x = 0; x < BLOCK_SIZE; x++) {
                for (y = 0; y < BLOCK_SIZE; y++) {
                    sum = 0.0;
                    for (u = 0; u < BLOCK_SIZE; u++) {
                        for (v = 0; v < BLOCK_SIZE; v++) {
                            // 係数のスケーリング
                            cu = (u == 0) ? 1.0 / sqrt(2.0) : 1.0;
                            cv = (v == 0) ? 1.0 / sqrt(2.0) : 1.0;

                            pixel_value = image[(row + v) * img_width + (col + u)];
                            sum += (
                                cu * cv * pixel_value 
                                * dct_table[x * BLOCK_SIZE + u] 
                                * dct_table[y * BLOCK_SIZE + v] 
                                / FACTOR / FACTOR
                             );
                        }
                    }

                    cff[((row + y) * img_width) + col + x] = (int)round(sum / sqrt(2 * BLOCK_SIZE));
                }
            }
        }
    }
}


