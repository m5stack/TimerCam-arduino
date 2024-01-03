#ifndef _M5_TIMER_CAM_POWER_H_
#define _M5_TIMER_CAM_POWER_H_

#include "Arduino.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "soc/adc_channel.h"
#include "stdlib.h"
#include "driver/ledc.h"
#include "RTC8563_Class.h"

#define POWER_HOLD_PIN 33
#define POWER_LED_PIN  2

#define BAT_ADC_PIN     38
#define BAT_ADC_CHANNEL ADC1_GPIO38_CHANNEL

#define BASE_VOLATAGE     3600
#define SCALE             0.661
#define ADC_FILTER_SAMPLE 64

class Power_Class {
   private:
   public:
    void begin();
    void powerOff();
    int16_t getBatteryVoltage();
    int16_t getBatteryLevel();
    void setLed(uint8_t brightness);
    void timerSleep(int seconds);
    void timerSleep(const rtc_time_t& time);
    void timerSleep(const rtc_date_t& date, const rtc_time_t& time);
};

#endif