/**
 * @file http_post.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief TimerCAM HTTP Post Test
 * @version 0.1
 * @date 2023-12-28
 *
 *
 * @Hardwares: TimerCAM
 * @Platform Version: Arduino M5Stack Board Manager v2.0.9
 * @Dependent Library:
 * TimerCam-arduino: https://github.com/m5stack/TimerCam-arduino
 * ArduinoHttpClient: https://github.com/arduino-libraries/ArduinoHttpClient
 */

#include "M5TimerCAM.h"
#include <WiFi.h>
#include <ArduinoHttpClient.h>

#define ssid     "ssid"
#define password "password"

#define SERVER "httpbin.org"

WiFiClient wifi;
HttpClient client = HttpClient(wifi, SERVER);

void setup() {
    TimerCAM.begin();

    if (!TimerCAM.Camera.begin()) {
        Serial.println("Camera Init Fail");
        return;
    }
    Serial.println("Camera Init Success");

    TimerCAM.Camera.sensor->set_pixformat(TimerCAM.Camera.sensor, PIXFORMAT_JPEG);
    // 2MP Sensor
    TimerCAM.Camera.sensor->set_framesize(TimerCAM.Camera.sensor, FRAMESIZE_UXGA);
    // 3MP Sensor
    // TimerCAM.Camera.sensor->set_framesize(TimerCAM.Camera.sensor, FRAMESIZE_QXGA);

    TimerCAM.Camera.sensor->set_vflip(TimerCAM.Camera.sensor, 1);
    TimerCAM.Camera.sensor->set_hmirror(TimerCAM.Camera.sensor, 0);

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
}

void loop() {
    if (TimerCAM.Camera.get()) {
        Serial.println("making POST request");

        String contentType = "image/jpeg";

        // client.post("/post", contentType, postData);
        client.post("/post", contentType.c_str(), TimerCAM.Camera.fb->len, TimerCAM.Camera.fb->buf);

        // read the status code and body of the response
        int statusCode  = client.responseStatusCode();
        String response = client.responseBody();

        Serial.print("Status code: ");
        Serial.println(statusCode);
        Serial.print("Response: ");
        Serial.println(response);

        Serial.println("Wait five seconds");
        TimerCAM.Camera.free();
        delay(5000);
    }
}
