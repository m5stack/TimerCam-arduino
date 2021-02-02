# Timer-CAM Library

English | [中文](README_cn.md)

## Description

This library provides some basic arduino cases for **TimerCAM**, including web surveillance cameras, sleep functions, etc. **TimerCAM**  is a camera module based on ESP32, integrated with ESP32 chip and 8M-PSRAM. The camera (ov3660) with 3 million pixels can view 66.5 ° and shoot 1600 x at most 1200 resolution photo, built-in LED status indicator, featuring ultra-low power consumption design. Through RTC (BM8563), timing sleep and wake-up can be realized. sleep current can be reduced to 2μA.The battery interface is reserved on the board, and users can access the battery power supply by themselves. The module supports WiFi image transmission and USB port debugging. The bottom HY2.0-4P port output can be connected to other peripherals.

## Applications

- Take pictures regularly
- Remote video monitoring

## PinMap

**Camera Interface PinMap**

| *Interface*             | *Camera Pin*| *TimerCamera*  |
| :-------------------  | :--------:| :------:  |
| SCCB Clock            | SIOC     |IO23        |
| SCCB Data             | SIOD     |IO25       |
| System Clock          | XCLK     |IO27       |
| Vertical Sync         | VSYNC    |IO22       |
| Horizontal Reference  | HREF     |IO26       |
| Pixel Clock           | PCLK     |IO21       |
| Pixel Data Bit 0      | D0       |IO32       |
| Pixel Data Bit 1      | D1       |IO35       |
| Pixel Data Bit 2      | D2       |IO34       |
| Pixel Data Bit 3      | D3       |IO5        |
| Pixel Data Bit 4      | D4       |IO39       |
| Pixel Data Bit 5      | D5       |IO18       |
| Pixel Data Bit 6      | D6       |IO36       |
| Pixel Data Bit 7      | D7       |IO19       |
| Camera Reset          | RESET    |IO15       |
| Camera Power Down     | PWDN     |-1         |
| Power Supply 3.3V     | 3V3      | 3V3       |
| Ground                | GND      | GND       |

**GROVE Interface**

| *Grove*         | *TimerCamera*  | 
| :-----------: | :------:  | 
| SCL           | IO13      | 
| SDA           | IO4       |
| 5V            | 5V        |
| GND           | GND       | 

**LED Interface**

| *LED*         | *TimerCamera*  |
| :-----------:| :------:  | 
| LED_Pin      | IO2     | 

**BAT Interface**

| *BAT*               | *TimerCamera*  |
| :------------------:| :-----------:  | 
| BAT_OUTPUT_HOLD_Pin | IO33           | 
| BAT_ADC_Pin         | IO38           | 

**BM8563 Interface**

*I2C Address: 0x51*

| *BM8563*       | *TimerCamera*  |
| :-------------:| :-----------:  | 
| BM8563_SDA_Pin | IO12           | 
| BM8563_SCL_Pin | IO14           | 

## Related Link

- **datasheet** 
   - [ESP32](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/esp32_datasheet_cn.pdf) 
   - [OV3660](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/unit/OV3660_CSP3_DS_1.3_sida.pdf)
