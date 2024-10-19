#ifndef __ETH_H__
#define __ETH_H__

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "pico/async_context_freertos.h"

// FreeRTOS Tasks
void vLaunchRxFunc(void *pvParameters);

void eth_init(void);
uint32_t eth_main(void);
void eth_tx_data(uint32_t *buf, uint32_t count);

#endif //__ETH_H__
