#include <Arduino.h>
#include "sdcard_server.h"
#include "mpu_server.h"
#include "wifi_server.h"
#include "web_server.h"
#include "gps_server.h"

// definitions
#define WIFI_SSID "linksys_mesh_2_4"
#define WIFI_PASS "QWE@123qwe"
#define SDCARD_CHIP_SELECT 5
#define GPSSerial Serial2

// objects
SdcardServer sdcardServer(SDCARD_CHIP_SELECT);
WifiServer wifiServer(WIFI_SSID, WIFI_PASS, BlinkLed::ENABLE);
GPSServer gpsServer(&GPSSerial);
MpuServer mpuServer;
WebServer webServer(mpuServer);

// taskhandles
TaskHandle_t webServerTask;

void webServerTaskFunction(void *parameter)
{
    while (1)
    {
        webServer.handleClient();
        vTaskDelay(pdMS_TO_TICKS(10)); // 10 ms delay
    }
}

// timing variables
unsigned long lastMpuTime = 0;
const float mpuFrequencyHz = 0.5;
const long mpuInterval = 1000 / mpuFrequencyHz;

unsigned long lasGpsTime = 0;
const float gpsFrequencyHz = 0.5;
const long gpsInterval = 1000 / gpsFrequencyHz;

uint32_t timer = millis();

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
    sdcardServer.writeFile("/test.txt", "Info\n");
}

void loop()
{
    wifiServer.blinkWifi();
    xTaskCreate(webServerTaskFunction, "webServerTask", 10000, NULL, 5, &webServerTask);

    if (millis() - lastMpuTime >= mpuInterval)
    {
        String mpuInfo = mpuServer.getData();
        Serial.println(mpuInfo);
        sdcardServer.appendFile("/test.txt", mpuInfo.c_str());

        lastMpuTime = millis();
    }

    if (millis() - lasGpsTime >= gpsInterval)
    {
        gpsServer.checkData();

        String gpsSatelites = "Satelites: " + gpsServer.getSatellites();
        String gpsTime = "Time: " + gpsServer.getTime();
        String gpsLocation = "Location: " + gpsServer.getLocation();

        Serial.println(gpsSatelites);
        Serial.println(gpsTime);
        Serial.println(gpsLocation);

        sdcardServer.appendFile("/test.txt", gpsTime.c_str());
        sdcardServer.appendFile("/test.txt", gpsLocation.c_str());

        lasGpsTime = millis();
    }
}
