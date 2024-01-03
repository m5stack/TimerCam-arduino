#ifndef _M5_TIMER_CAM_H_
#define _M5_TIMER_CAM_H_

#include "./utility/Power_Class.h"
#include "./utility/RTC8563_Class.h"
#include "./utility/Camera_Class.h"
#include "esp_camera.h"

namespace m5 {
class M5TimerCAM {
   private:
    /* data */
   public:
    void begin(bool enableRTC = false);
    Power_Class Power;
    RTC8563_Class Rtc;
    Camera_Class Camera;
};
}  // namespace m5
extern m5::M5TimerCAM TimerCAM;

#endif