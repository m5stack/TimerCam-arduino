/**
 * @file ext_wakeup.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief TimerCAM Ext Wakeup Test
 * @version 0.1
 * @date 2024-04-08
 *
 *
 * @Hardwares: TimerCAM
 * @Platform Version: Arduino M5Stack Board Manager v2.1.0
 * @Dependent Library:
 * TimerCam-arduino: https://github.com/m5stack/TimerCam-arduino
 */

#include "M5TimerCAM.h"

#define CAM_EXT_WAKEUP_PIN 4

void led_breathe(int ms) {
    for (int16_t i = 0; i < 255; i++) {
        TimerCAM.Power.setLed(i);
        vTaskDelay(pdMS_TO_TICKS(ms));
    }

    for (int16_t i = 255; i >= 0; i--) {
        TimerCAM.Power.setLed(i);
        vTaskDelay(pdMS_TO_TICKS(ms));
    }
}

void setup() {
    TimerCAM.begin(true);
    Serial.println("Wake up!!!");
    led_breathe(5);

    if (!TimerCAM.Camera.begin()) {
        Serial.println("Camera Init Fail");
        return;
    }
    Serial.println("Camera Init Success");
    TimerCAM.Camera.sensor->set_pixformat(TimerCAM.Camera.sensor,
                                          PIXFORMAT_JPEG);
    TimerCAM.Camera.sensor->set_framesize(TimerCAM.Camera.sensor,
                                          FRAMESIZE_QVGA);

    TimerCAM.Camera.sensor->set_vflip(TimerCAM.Camera.sensor, 1);
    TimerCAM.Camera.sensor->set_hmirror(TimerCAM.Camera.sensor, 0);

    if (TimerCAM.Camera.get()) {
        Serial.printf("pic size: %d\n", TimerCAM.Camera.fb->len);
        TimerCAM.Camera.free();
    }

    TimerCAM.Camera.deinit();

    // sleep after 5s wakeup!
    // TimerCAM.Power.timerSleep(5);
    gpio_hold_en((gpio_num_t)POWER_HOLD_PIN);
    gpio_deep_sleep_hold_en();
    esp_sleep_enable_ext0_wakeup((gpio_num_t)CAM_EXT_WAKEUP_PIN,
                                 1);  // 1 = High, 0 = Low

    while (digitalRead(CAM_EXT_WAKEUP_PIN) == HIGH) {
        // wait for singal to go low
        delay(1);
    }

    // Go to sleep now
    Serial.println("Going to sleep now");
    esp_deep_sleep_start();
}

void loop() {
}
