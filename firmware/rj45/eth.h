#ifndef __ETH_H__
#define __ETH_H__

#include <stdint.h>

void eth_init(void);
uint32_t eth_main(void);
void eth_tx_data(uint32_t *buf, uint32_t count);

#endif //__ETH_H__
