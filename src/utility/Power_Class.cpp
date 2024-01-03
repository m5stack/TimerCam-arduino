#include "Power_Class.h"
#include "M5TimerCAM.h"

static esp_adc_cal_characteristics_t* adc_chars;

void Power_Class::begin() {
    pinMode(POWER_HOLD_PIN, OUTPUT);
    digitalWrite(POWER_HOLD_PIN, HIGH);

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(BAT_ADC_CHANNEL, ADC_ATTEN_DB_11);
    adc_chars = (esp_adc_cal_characteristics_t*)calloc(
        1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12,
                             BASE_VOLATAGE, adc_chars);

    ledc_timer_config_t ledc_timer = {
        .speed_mode      = LEDC_HIGH_SPEED_MODE,  // timer mode
        .duty_resolution = LEDC_TIMER_10_BIT,     // resolution of PWM duty
        .timer_num       = LEDC_TIMER_2,          // timer index
        .freq_hz         = 256,                   // frequency of PWM signal
    };

    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .gpio_num   = POWER_LED_PIN,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel    = LEDC_CHANNEL_6,
        .timer_sel  = LEDC_TIMER_2,
        .duty       = 0,

    };
    ledc_channel_config(&ledc_channel);
}

void Power_Class::setLed(uint8_t brightness) {
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, brightness);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);
}

int16_t Power_Class::getBatteryVoltage() {
    uint32_t adc_raw_value = 0;
    for (uint16_t i = 0; i < ADC_FILTER_SAMPLE; i++) {
        adc_raw_value += adc1_get_raw(BAT_ADC_CHANNEL);
    }

    adc_raw_value = adc_raw_value / ADC_FILTER_SAMPLE;
    uint32_t voltage =
        (uint32_t)(esp_adc_cal_raw_to_voltage(adc_raw_value, adc_chars) /
                   SCALE);
    return voltage;
}

int16_t Power_Class::getBatteryLevel() {
    int16_t voltage = getBatteryVoltage();
    int level       = (voltage - 3300) * 100 / (float)(4150 - 3350);

    return (level < 0) ? 0 : (level >= 100) ? 100 : level;
}

void Power_Class::powerOff() {
    digitalWrite(POWER_HOLD_PIN, LOW);
    esp_deep_sleep_start();
}

void Power_Class::timerSleep(int seconds) {
    TimerCAM.Rtc.disableIRQ();
    TimerCAM.Rtc.setAlarmIRQ(seconds);
    powerOff();
}
void Power_Class::timerSleep(const rtc_time_t& time) {
    TimerCAM.Rtc.disableIRQ();
    TimerCAM.Rtc.setAlarmIRQ(time);
    powerOff();
}
void Power_Class::timerSleep(const rtc_date_t& date, const rtc_time_t& time) {
    TimerCAM.Rtc.disableIRQ();
    TimerCAM.Rtc.setAlarmIRQ(date, time);
    powerOff();
}