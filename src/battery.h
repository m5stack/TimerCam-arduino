#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

// Init bat hold gpio, adc gpio
void bat_init();

// Enable battery power supply if USB is unplugged
void bat_hold_output();

// Disable battery power supply if USB is unplugged
void bat_disable_output();

// return: 0 ~ 4096
uint32_t bat_get_adc_raw();

// return: xxxx mv
uint32_t bat_get_voltage();

#ifdef __cplusplus
}
#endif