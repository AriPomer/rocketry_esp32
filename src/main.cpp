#include <Arduino.h>
#include "sdcard_server.h"
#include "mpu_server.h"
#include "wifi_server.h"
#include "web_server.h"
#include "gps_server.h"

#define WIFI_SSID "linksys_mesh_2_4"
#define WIFI_PASS "QWE@123qwe"
#define GPSSerial Serial1
#define CHIP_SELECT 5
#define GPSSerial Serial1

SdcardServer sdcardServer(CHIP_SELECT);
WifiServer wifiServer(WIFI_SSID, WIFI_PASS, BlinkLed::ENABLE);
GPSServer gpsServer(GPSSerial);
MpuServer mpuServer;
WebServer webServer(mpuServer);

unsigned long lastMpuTime = 0;
const float mpuFrequencyHz = 0.5;
const long mpuInterval = 1000 / mpuFrequencyHz ;

unsigned long lasGpsTime = 0;
const float gpsFrequencyHz = 0.5;
const long gpsInterval = 1000 / gpsFrequencyHz ;

void setup()
{
    Serial.begin(115200);

    sdcardServer.begin();
    wifiServer.begin();
    gpsServer.begin();
    mpuServer.begin();
    webServer.begin();

    sdcardServer.begin();
    sdcardServer.deleteFile("/test.txt");
    sdcardServer.writeFile("/test.txt", "Mpu6050 Info \n");
}

void loop()
{
    wifiServer.blinkWifi();
    webServer.handleClient();

    if (millis() - lastMpuTime >= mpuInterval)
    {
        String mpuInfo = mpuServer.getData();
        Serial.println(mpuInfo);
        sdcardServer.appendFile("/test.txt", mpuInfo.c_str());

        lastMpuTime = millis();
    }

    if (millis() - lasGpsTime >= gpsInterval)
    {
        String gpsTime = "Time: " + gpsServer.getTime() + " Location: " + gpsServer.getLocation();
        Serial.println(gpsTime);
        sdcardServer.appendFile("/test.txt", gpsTime.c_str());

        lasGpsTime = millis();
    }

}
