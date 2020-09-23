#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "camera_pins.h"
#include "battery.h"
#include "led.h"
#include "bmm8563.h"

void led_breathe_test() {
  for (int16_t i = 0; i < 1024; i++) {
    led_brightness(i);
    vTaskDelay(pdMS_TO_TICKS(1));
  }

  for (int16_t i = 1023; i >= 0; i--) {
    led_brightness(i);
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

void setup() {
  Serial.begin(115200);

  // will hold bat output
  bat_init();

  led_init(CAMERA_LED_GPIO);
  bmm8563_init();

  // 5 sec later will wake up
  bmm8563_setTimerIRQ(5);
  
  // rtc_date_t date;
  // date.year = 2020;
  // date.month = 9;
  // date.day = 20;
  // date.hour = 15;
  // date.minute = 59;
  // date.second = 06;
  // bmm8563_setTime(&date);
  led_breathe_test();
}

void loop() {
  rtc_date_t date;
  bmm8563_getTime(&date);
  Serial.printf("Time: %d/%d/%d %02d:%02d:%-2d\r\n", date.year, date.month, date.day, date.hour, date.minute, date.second);
  Serial.printf("volt: %d mv\r\n", bat_get_voltage());

  // disable bat output, will wake up after 5 sec, Sleep current is 1~2μA
  bat_disable_output();

  // if usb not connect, will not in here;
  esp_deep_sleep(5000000);
  esp_deep_sleep_start();
}
