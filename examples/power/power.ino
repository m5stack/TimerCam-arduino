/**
 * @file power.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief TimerCAM Battery Test
 * @version 0.1
 * @date 2024-01-02
 *
 *
 * @Hardwares: TimerCAM
 * @Platform Version: Arduino M5Stack Board Manager v2.0.9
 * @Dependent Library:
 * TimerCam-arduino: https://github.com/m5stack/TimerCam-arduino
 */

#include "M5TimerCAM.h"

void setup() {
    TimerCAM.begin(true);
}

void loop() {
    Serial.printf("Bat Voltage: %dmv\r\n", TimerCAM.Power.getBatteryVoltage());
    Serial.printf("Bat Level: %d%%\r\n", TimerCAM.Power.getBatteryLevel());
    delay(1000);
}
