#include "OV2640.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>

#include "SimStreamer.h"
#include "OV2640Streamer.h"
#include "CRtspSession.h"

#include "battery.h"

#define ssid "ssid"
#define password "password"

OV2640 cam;

WiFiServer rtspServer(8554);
CStreamer *streamer;

void setup()
{

    bat_init();
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH); 
    Serial.begin(115200);
    while (!Serial)
    {
        ;
    }
    
    camera_config_t timercam_config{

        .pin_pwdn = -1,
        .pin_reset = 15,
        .pin_xclk = 27,
        .pin_sscb_sda = 25,
        .pin_sscb_scl = 23,

        .pin_d7 = 19,
        .pin_d6 = 36,
        .pin_d5 = 18,
        .pin_d4 = 39,
        .pin_d3 = 5,
        .pin_d2 = 34,
        .pin_d1 = 35,
        .pin_d0 = 32,
        .pin_vsync = 22,
        .pin_href = 26,
        .pin_pclk = 21,
        .xclk_freq_hz = 20000000,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,
        .pixel_format = PIXFORMAT_JPEG,
        .frame_size = FRAMESIZE_VGA,
        .jpeg_quality = 12,               //0-63 lower numbers are higher quality
        .fb_count = 2 // if more than one i2s runs in continous mode.  Use only with jpeg
    };


    cam.init(timercam_config);

    sensor_t *s = esp_camera_sensor_get();
    s->set_vflip(s, 1);
    s->set_hmirror(s, 0);

    IPAddress ip;

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(F("."));
    }
    ip = WiFi.localIP();
    Serial.println(F("WiFi connected"));
    Serial.print("RTSP URL: rtsp://");
    Serial.print(ip);
    Serial.println(":8554/mjpeg/1");
    rtspServer.begin();
    //streamer = new SimStreamer(true);             // our streamer for UDP/TCP based RTP transport
    streamer = new OV2640Streamer(cam);             // our streamer for UDP/TCP based RTP transport
}

void loop()
{
    // If we have an active client connection, just service that until gone
    streamer->handleRequests(0); // we don't use a timeout here,
    // instead we send only if we have new enough frames
    uint32_t now = millis();
    if(streamer->anySessions()) {
        streamer->streamImage(now);
    }
    
    WiFiClient rtspClient = rtspServer.accept();
    if(rtspClient) {
        Serial.print("client: ");
        Serial.print(rtspClient.remoteIP());
        Serial.println();
        streamer->addSession(rtspClient);
    }
}
