#include <Arduino.h>
#include "sdcard_server.h"
#include "mpu_server.h"
#include "wifi_server.h"
#include "web_server.h"
#include "gps_server.h"

#define WIFI_SSID "linksys_mesh_2_4"
#define WIFI_PASS "QWE@123qwe"
#define CHIP_SELECT 5
#define GPSSerial Serial2

SdcardServer sdcardServer(CHIP_SELECT);
WifiServer wifiServer(WIFI_SSID, WIFI_PASS, BlinkLed::ENABLE);
GPSServer gpsServer(GPSSerial);
MpuServer mpuServer;
WebServer webServer(mpuServer);

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
        char c = gpsServer.GPS.read();

        if (gpsServer.GPS.newNMEAreceived())
        {
            Serial.print(gpsServer.GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
            if (!gpsServer.GPS.parse(gpsServer.GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
                return;                     // we can fail to parse a sentence in which case we should just wait for another
        }

        // approximately every 2 seconds or so, print out the current stats
        if (millis() - timer > 2000)
        {
            timer = millis(); // reset the timer
            Serial.print("\nTime: ");
            if (gpsServer.GPS.hour < 10)
            {
                Serial.print('0');
            }
            Serial.print(gpsServer.GPS.hour, DEC);
            Serial.print(':');
            if (gpsServer.GPS.minute < 10)
            {
                Serial.print('0');
            }
            Serial.print(gpsServer.GPS.minute, DEC);
            Serial.print(':');
            if (gpsServer.GPS.seconds < 10)
            {
                Serial.print('0');
            }
            Serial.print(gpsServer.GPS.seconds, DEC);
            Serial.print('.');
            if (gpsServer.GPS.milliseconds < 10)
            {
                Serial.print("00");
            }
            else if (gpsServer.GPS.milliseconds > 9 && gpsServer.GPS.milliseconds < 100)
            {
                Serial.print("0");
            }
            Serial.println(gpsServer.GPS.milliseconds);
            Serial.print("Date: ");
            Serial.print(gpsServer.GPS.day, DEC);
            Serial.print('/');
            Serial.print(gpsServer.GPS.month, DEC);
            Serial.print("/20");
            Serial.println(gpsServer.GPS.year, DEC);
            Serial.print("Fix: ");
            Serial.print((int)gpsServer.GPS.fix);
            Serial.print(" quality: ");
            Serial.println((int)gpsServer.GPS.fixquality);
            if (gpsServer.GPS.fix)
            {
                Serial.print("Location: ");
                Serial.print(gpsServer.GPS.latitude, 4);
                Serial.print(gpsServer.GPS.lat);
                Serial.print(", ");
                Serial.print(gpsServer.GPS.longitude, 4);
                Serial.println(gpsServer.GPS.lon);
                Serial.print("Speed (knots): ");
                Serial.println(gpsServer.GPS.speed);
                Serial.print("Angle: ");
                Serial.println(gpsServer.GPS.angle);
                Serial.print("Altitude: ");
                Serial.println(gpsServer.GPS.altitude);
                Serial.print("Satellites: ");
                Serial.println((int)gpsServer.GPS.satellites);
                Serial.print("Antenna status: ");
                Serial.println((int)gpsServer.GPS.antenna);
            }
        }
    }
}
