/* LEDC (LED Controller) fade example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define LEDC_HS_TIMER           LEDC_TIMER_2
#define LEDC_HS_MODE            LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_CHANNEL     LEDC_CHANNEL_5    

void led_init(int led_gpio) {
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_10_BIT, // resolution of PWM duty
        .freq_hz = 5000,              // frequency of PWM signal
        .speed_mode = LEDC_HS_MODE,   // timer mode
        .timer_num = LEDC_HS_TIMER    // timer index
    };

    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);
    
    ledc_channel_config_t ledc_channel = 
    {
        .channel    = LEDC_HS_CH0_CHANNEL,
        .duty       = 1024,
        .gpio_num   = led_gpio,
        .speed_mode = LEDC_HS_MODE,
        .timer_sel  = LEDC_HS_TIMER
    };
    ledc_channel_config(&ledc_channel);
}

void led_brightness(int duty) {
    ledc_set_duty(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL, duty);
    ledc_update_duty(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL);
}