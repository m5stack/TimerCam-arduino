# Timer-CAM Library

[English](README.md) | 中文

## 描述

该库为**TimerCAM**产品系列提供了一些arduino基础案例，包含web监控摄像头，休眠功能等内容，**TimerCAM**是一款基于ESP32的摄像头模块，集成ESP32芯片，板载8M PSRAM，采用300万像素的摄像头（OV3660）可视角66.5°，最高可实现拍摄1600 x 1200分辨率的照片，带有状态指示灯，主打超低功耗设计，通过RTC(BM8563)可实现定时休眠与唤醒，休眠电流可降低至2μA，板上预留电池接口，用户可自行接入电池供电。模块支持WiFi图像传输和USB端口调试，底部HY2.0-4P端口输出，可连接其他外设。
   
## 应用

- 定时拍照
- 远程监控

## 管脚映射

**摄像头驱动芯片 OV3660 接口**

| *接口*             | *Camera Pin*| *TimerCamera*  |
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

**GROVE 接口**

| *Grove*         | *TimerCamera*  | 
| :-----------: | :------:  | 
| SCL           | IO13      | 
| SDA           | IO4       |
| 5V            | 5V        |
| GND           | GND       | 

**LED 接口**

| *LED*         | *TimerCamera*  |
| :-----------:| :------:  | 
| LED_Pin      | IO2     | 

**BAT 接口**

| *BAT*         | *TimerCamera*  |
| :-----------:| :------:  | 
| BAT_ADC_Pin     | IO33     | 

## 相关链接

- **数据手册** 
   - [ESP32](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/esp32_datasheet_cn.pdf) 
   - [OV3660](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/unit/OV3660_CSP3_DS_1.3_sida.pdf)