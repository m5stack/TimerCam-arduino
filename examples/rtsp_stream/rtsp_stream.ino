/**
 * @file rtsp_stream.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief TimerCAM RTSP Stream
 * @version 0.1
 * @date 2023-12-28
 *
 *
 * @Hardwares: TimerCAM
 * @Platform Version: Arduino M5Stack Board Manager v2.0.9
 * @Dependent Library:
 * TimerCam-arduino: https://github.com/m5stack/TimerCam-arduino
 * Micro-RTSP: https://github.com/geeksville/Micro-RTSP
 */

#include "M5TimerCAM.h"
#include <WiFi.h>
#include "CStreamer.h"

#define ssid     "ssid"
#define password "password"

class TimerCamRTSP : public CStreamer {
    Camera_Class &_camera;

   public:
    TimerCamRTSP(Camera_Class &camera, int width, int height) : CStreamer(width, height), _camera(camera){};
    virtual void streamImage(uint32_t curMsec) {
        if (_camera.get()) {
            streamFrame(_camera.fb->buf, _camera.fb->len, millis());
            _camera.free();
        }
    }
};

TimerCamRTSP *streamer;
WiFiServer server(8554);

void setup() {
    TimerCAM.begin();

    if (!TimerCAM.Camera.begin()) {
        Serial.println("Camera Init Fail");
        return;
    }
    Serial.println("Camera Init Success");

    TimerCAM.Camera.sensor->set_pixformat(TimerCAM.Camera.sensor, PIXFORMAT_JPEG);
    TimerCAM.Camera.sensor->set_framesize(TimerCAM.Camera.sensor, FRAMESIZE_QVGA);

    // 2MP Sensor
    // TimerCAM.Camera.sensor->set_framesize(TimerCAM.Camera.sensor, FRAMESIZE_UXGA);
    // 3MP Sensor
    // TimerCAM.Camera.sensor->set_framesize(TimerCAM.Camera.sensor, FRAMESIZE_QXGA);

    TimerCAM.Camera.sensor->set_vflip(TimerCAM.Camera.sensor, 1);
    TimerCAM.Camera.sensor->set_hmirror(TimerCAM.Camera.sensor, 0);

    if (TimerCAM.Camera.get()) {
        int width  = TimerCAM.Camera.fb->width;
        int height = TimerCAM.Camera.fb->height;
        streamer   = new TimerCamRTSP(TimerCAM.Camera, width, height);
        TimerCAM.Camera.free();
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.setSleep(false);
    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(ssid);
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");

    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.print("RTSP URL: rtsp://");
    Serial.print(WiFi.localIP());
    Serial.println(":8554/mjpeg/1");

    server.begin();
}

void loop() {
    uint32_t msecPerFrame     = 100;
    static uint32_t lastimage = millis();

    // If we have an active client connection, just service that until gone
    streamer->handleRequests(0);  // we don't use a timeout here,
    // instead we send only if we have new enough frames
    uint32_t now = millis();
    if (streamer->anySessions()) {
        if (now > lastimage + msecPerFrame || now < lastimage) {  // handle clock rollover
            streamer->streamImage(now);
            lastimage = now;

            // check if we are overrunning our max frame rate
            now = millis();
            if (now > lastimage + msecPerFrame) {
                Serial.printf("warning exceeding max frame rate of %d ms\n", now - lastimage);
            }
        }
    }

    WiFiClient rtspClient = server.accept();
    if (rtspClient) {
        Serial.print("client: ");
        Serial.print(rtspClient.remoteIP());
        Serial.println();
        streamer->addSession(rtspClient);
    }
}
