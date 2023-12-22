#include <Arduino.h>
#include "sdcard_server.h"
#include "mpu_server.h"
#include "wifi_server.h"
#include "web_server.h"

#define WIFI_SSID "Partner_95CF"
#define WIFI_PASS "18664317"

WifiServer wifiServer(WIFI_SSID, WIFI_PASS, BlinkLed::ENABLE);
SdcardServer sdcardServer(5);
MpuServer mpuServer;
WebServer webServer;

void setup()
{
    Serial.begin(115200);

    wifiServer.begin();
    webServer.begin();
    sdcardServer.begin();
    mpuServer.begin();
}

void loop()
{
    webServer.handleClient();
    wifiServer.blinkWifi();

    String mpuInfo = mpuServer.getData();
    Serial.println(mpuInfo);

}
