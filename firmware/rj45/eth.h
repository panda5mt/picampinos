#ifndef __ETH_H__
#define __ETH_H__

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "pico/async_context_freertos.h"

TaskHandle_t rxhandle;

void eth_init(void);
uint32_t eth_main(void);
void eth_tx_data(uint32_t *buf, uint32_t count);
void vLaunchRxFunc(void *pvParameters);
#endif //__ETH_H__
