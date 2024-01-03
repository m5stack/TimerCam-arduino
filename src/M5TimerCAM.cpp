#include "M5TimerCAM.h"

using namespace m5;

M5TimerCAM TimerCAM;

void M5TimerCAM::begin(bool enableRTC) {
    Power.begin();
    Serial.begin(115200);
    if (enableRTC) {
        Rtc.begin();
    }
}
