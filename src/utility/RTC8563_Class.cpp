// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#include "RTC8563_Class.h"

#include <stdlib.h>

tm rtc_datetime_t::get_tm(void) {
    tm t_st = {
        time.seconds,     time.minutes, time.hours, date.date, date.month - 1,
        date.year - 1900, date.weekDay, 0,          0,
    };
    return t_st;
}

void rtc_datetime_t::set_tm(tm& datetime) {
    date = rtc_date_t{datetime};
    time = rtc_time_t{datetime};
}

static std::uint8_t bcd2ToByte(std::uint8_t value) {
    return ((value >> 4) * 10) + (value & 0x0F);
}

static std::uint8_t byteToBcd2(std::uint8_t value) {
    std::uint_fast8_t bcdhigh = value / 10;
    return (bcdhigh << 4) | (value - (bcdhigh * 10));
}

bool RTC8563_Class::begin() {
    _i2c.begin(&Wire, 12, 14);
    _i2c.writeByte(BM8563_I2C_ADDR, 0x00, 0x00);
    return _i2c.writeByte(BM8563_I2C_ADDR, 0x0E, 0x03);
    ;
}

bool RTC8563_Class::getVoltLow(void) {
    return _i2c.readByte(BM8563_I2C_ADDR, 0x02) & 0x80;  // RTCC_VLSEC_MASK
}

bool RTC8563_Class::getDateTime(rtc_datetime_t* datetime) {
    std::uint8_t buf[7] = {0};

    if (!_i2c.readBytes(BM8563_I2C_ADDR, 0x02, buf, 7)) {
        return false;
    }

    datetime->time.seconds = bcd2ToByte(buf[0] & 0x7f);
    datetime->time.minutes = bcd2ToByte(buf[1] & 0x7f);
    datetime->time.hours   = bcd2ToByte(buf[2] & 0x3f);

    datetime->date.date    = bcd2ToByte(buf[3] & 0x3f);
    datetime->date.weekDay = bcd2ToByte(buf[4] & 0x07);
    datetime->date.month   = bcd2ToByte(buf[5] & 0x1f);
    datetime->date.year =
        bcd2ToByte(buf[6] & 0xff) + ((0x80 & buf[5]) ? 1900 : 2000);
    return true;
}

bool RTC8563_Class::getTime(rtc_time_t* time) {
    std::uint8_t buf[3] = {0};

    if (!_i2c.readBytes(BM8563_I2C_ADDR, 0x02, buf, 3)) {
        return false;
    }

    time->seconds = bcd2ToByte(buf[0] & 0x7f);
    time->minutes = bcd2ToByte(buf[1] & 0x7f);
    time->hours   = bcd2ToByte(buf[2] & 0x3f);
    return true;
}

void RTC8563_Class::setTime(const rtc_time_t& time) {
    std::uint8_t buf[] = {byteToBcd2(time.seconds), byteToBcd2(time.minutes),
                          byteToBcd2(time.hours)};
    _i2c.writeBytes(BM8563_I2C_ADDR, 0x02, buf, sizeof(buf));
}

bool RTC8563_Class::getDate(rtc_date_t* date) {
    std::uint8_t buf[4] = {0};

    if (!_i2c.readBytes(BM8563_I2C_ADDR, 0x05, buf, 4)) {
        return false;
    }

    date->date    = bcd2ToByte(buf[0] & 0x3f);
    date->weekDay = bcd2ToByte(buf[1] & 0x07);
    date->month   = bcd2ToByte(buf[2] & 0x1f);
    date->year    = bcd2ToByte(buf[3] & 0xff) + ((0x80 & buf[2]) ? 1900 : 2000);
    return true;
}

void RTC8563_Class::setDate(const rtc_date_t& date) {
    std::uint8_t w = date.weekDay;
    if (w > 6 && date.year >= 1900 &&
        ((std::size_t)(date.month - 1)) < 12) {  /// weekDay auto adjust
        int32_t year  = date.year;
        int32_t month = date.month;
        int32_t day   = date.date;
        if (month < 3) {
            year--;
            month += 12;
        }
        int32_t ydiv100 = year / 100;
        w               = (year + (year >> 2) - ydiv100 + (ydiv100 >> 2) +
             (13 * month + 8) / 5 + day) %
            7;
    }

    std::uint8_t buf[] = {
        byteToBcd2(date.date), w,
        (std::uint8_t)(byteToBcd2(date.month) + (date.year < 2000 ? 0x80 : 0)),
        byteToBcd2(date.year % 100)};
    _i2c.writeBytes(BM8563_I2C_ADDR, 0x05, buf, sizeof(buf));
}

int RTC8563_Class::setAlarmIRQ(int afterSeconds) {
    std::uint8_t reg_value = _i2c.readByte(BM8563_I2C_ADDR, 0x01) & ~0x0C;

    if (afterSeconds < 0) {  // disable timer
        _i2c.writeByte(BM8563_I2C_ADDR, 0x01, reg_value & ~0x01);
        _i2c.writeByte(BM8563_I2C_ADDR, 0x0E, 0x03);
        return -1;
    }

    std::size_t div         = 1;
    std::uint8_t type_value = 0x82;
    if (afterSeconds < 270) {
        if (afterSeconds > 255) {
            afterSeconds = 255;
        }
    } else {
        div          = 60;
        afterSeconds = (afterSeconds + 30) / div;
        if (afterSeconds > 255) {
            afterSeconds = 255;
        }
        type_value = 0x83;
    }

    _i2c.writeByte(BM8563_I2C_ADDR, 0x0E, type_value);
    _i2c.writeByte(BM8563_I2C_ADDR, 0x0F, afterSeconds);

    _i2c.writeByte(BM8563_I2C_ADDR, 0x01, (reg_value | 0x01) & ~0x80);
    return afterSeconds * div;
}

int RTC8563_Class::setAlarmIRQ(const rtc_time_t& time) {
    union {
        std::uint32_t raw = ~0;
        std::uint8_t buf[4];
    };
    bool irq_enable = false;

    if (time.minutes >= 0) {
        irq_enable = true;
        buf[0]     = byteToBcd2(time.minutes) & 0x7f;
    }

    if (time.hours >= 0) {
        irq_enable = true;
        buf[1]     = byteToBcd2(time.hours) & 0x3f;
    }

    _i2c.writeBytes(BM8563_I2C_ADDR, 0x09, buf, 4);

    if (irq_enable) {
        _i2c.writeBitOn(BM8563_I2C_ADDR, 0x01, 0x02);
    } else {
        _i2c.writeBitOff(BM8563_I2C_ADDR, 0x01, 0x02);
    }

    return irq_enable;
}

int RTC8563_Class::setAlarmIRQ(const rtc_date_t& date, const rtc_time_t& time) {
    union {
        std::uint32_t raw = ~0;
        std::uint8_t buf[4];
    };
    bool irq_enable = false;

    if (time.minutes >= 0) {
        irq_enable = true;
        buf[0]     = byteToBcd2(time.minutes) & 0x7f;
    }

    if (time.hours >= 0) {
        irq_enable = true;
        buf[1]     = byteToBcd2(time.hours) & 0x3f;
    }

    if (date.date >= 0) {
        irq_enable = true;
        buf[2]     = byteToBcd2(date.date) & 0x3f;
    }

    if (date.weekDay >= 0) {
        irq_enable = true;
        buf[3]     = byteToBcd2(date.weekDay) & 0x07;
    }

    _i2c.writeBytes(BM8563_I2C_ADDR, 0x09, buf, 4);

    if (irq_enable) {
        _i2c.writeBitOn(BM8563_I2C_ADDR, 0x01, 0x02);

    } else {
        _i2c.writeBitOff(BM8563_I2C_ADDR, 0x01, 0x02);
    }

    return irq_enable;
}

bool RTC8563_Class::getIRQstatus(void) {
    return (0x0C & _i2c.readByte(BM8563_I2C_ADDR, 0x01));
}

void RTC8563_Class::clearIRQ(void) {
    _i2c.writeBitOff(BM8563_I2C_ADDR, 0x01, 0x0C);
}

void RTC8563_Class::disableIRQ(void) {
    // disable alerm (bit7:1=disabled)
    std::uint8_t buf[4] = {0x80, 0x80, 0x80, 0x80};
    _i2c.writeBytes(BM8563_I2C_ADDR, 0x09, buf, 4);

    // disable timer (bit7:0=disabled)
    _i2c.writeByte(BM8563_I2C_ADDR, 0x0E, 0);

    // clear flag and INT enable bits
    _i2c.writeByte(BM8563_I2C_ADDR, 0x01, 0x00);
}

void RTC8563_Class::setSystemTimeFromRtc(struct timezone* tz) {
#if !defined(M5UNIFIED_PC_BUILD)
    rtc_datetime_t dt;
    if (getDateTime(&dt)) {
        tm t_st;
        t_st.tm_isdst = -1;
        t_st.tm_year  = dt.date.year - 1900;
        t_st.tm_mon   = dt.date.month - 1;
        t_st.tm_mday  = dt.date.date;
        t_st.tm_hour  = dt.time.hours;
        t_st.tm_min   = dt.time.minutes;
        t_st.tm_sec   = dt.time.seconds;
        timeval now;
        // mktime(3) uses localtime, force UTC
        char* oldtz = getenv("TZ");
        setenv("TZ", "GMT0", 1);
        tzset();  // Workaround for
                  // https://github.com/espressif/esp-idf/issues/11455
        now.tv_sec = mktime(&t_st);
        if (oldtz) {
            setenv("TZ", oldtz, 1);
        } else {
            unsetenv("TZ");
        }
        now.tv_usec = 0;
        settimeofday(&now, tz);
    }
#endif
}
