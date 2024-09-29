
#ifndef _PICO_PSRAM_H_
#define _PICO_PSRAM_H_
#include <stdint.h>
#include <stdlib.h>

/// @brief The setup_psram function - note that this is not in flash
///
/// @param psram_cs_pin The pin that the PSRAM is connected to
/// @return size_t The size of the PSRAM
///
size_t pico_setup_psram(uint32_t psram_cs_pin);

/// @brief The sfe_psram_update_timing function - note that this is not in flash
///
/// @note - updates the PSRAM QSPI timing - call if the system clock is changed after PSRAM is initialized
///
void pico_psram_update_timing(void);
#endif