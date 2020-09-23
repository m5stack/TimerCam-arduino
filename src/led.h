#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// 0 ~ 1024
void led_brightness(int duty);

void led_init(int led_gpio);

#ifdef __cplusplus
}
#endif