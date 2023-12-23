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
WebServer webServer(mpuServer);

unsigned long currentTime = 0;

unsigned long lastMpuPrintTime = 0;
const float mpuFrequencyHz = 0.5;
const long mpuPrintInterval = 1000 / mpuFrequencyHz ;

void setup()
{
    Serial.begin(115200);

    mpuServer.begin();
    wifiServer.begin();
    webServer.begin();

    sdcardServer.begin();
    sdcardServer.deleteFile("/test.txt");
    sdcardServer.writeFile("/test.txt", "Mpu6050 Info \n");
}

void loop()
{
    currentTime = millis();

    wifiServer.blinkWifi();
    webServer.handleClient();

    if (currentTime - lastMpuPrintTime >= mpuPrintInterval)
    {
        String mpuInfo = mpuServer.getData();
        Serial.println(mpuInfo);
        sdcardServer.appendFile("/test.txt", mpuInfo.c_str());

        lastMpuPrintTime = currentTime;
    }


}
