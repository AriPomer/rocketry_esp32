#include <Arduino.h>
#include "sdcard_server.h"
#include "mpu_server.h"
#include "gps_server.h"
#include "mpl_server.h"

#include <XBee.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// definitions
const char* ssid = "rocket_esp32";
const char* udpBroadcastIP = "255.255.255.255";
const int udpServerPort = 5555;

#define SDCARD_CHIP_SELECT 5
#define SeaLevelPressure 1013.26

// objects
SdcardServer sdcardServer(SDCARD_CHIP_SELECT);
GPSServer gpsServer;
MpuServer mpuServer; 
MPLSserver mplServer;
WiFiUDP udp;

// taskhandles
// TaskHandle_t webServerTask;

unsigned long lastSentMsgTime = 0;
const unsigned long interval = 0;


void setup()
{
    Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8N1, 9, 10);

    sdcardServer.begin();
    gpsServer.begin();
    mpuServer.begin();
    mplServer.begin(SeaLevelPressure);
    
    sdcardServer.begin();
    sdcardServer.deleteFile("/test.txt");
    sdcardServer.writeFile("/test.txt", "");
}

void broadcast_udp(const String& msg)
{
  udp.beginPacket(udpBroadcastIP, udpServerPort);
  udp.print(msg);
  udp.endPacket();
}

void loop()
{
    unsigned long timeNow = millis();

    Serial.println("Sending to Serial1");
    Serial1.println("Hello from Serial1");

    delay(2000);


    if (timeNow - lastSentMsgTime >= interval)
    {
        String altMsg = "'altitude': '" + mplServer.getAltitude() + "'";

        gpsServer.checkData();
        String gpsLatMsg = "'lat': '" + String(gpsServer.GPS.latitude, 4) + "'";
        String gpsLonMsg = "'lon': '" + String(gpsServer.GPS.longitude, 4) + "'";

        // send udp messages
        broadcast_udp(altMsg);
        broadcast_udp(gpsLatMsg);
        broadcast_udp(gpsLonMsg);

        // send xbee messages
        Serial1.println(altMsg);
        Serial1.println(gpsLatMsg);
        Serial1.println(gpsLonMsg);

        // save to sdcard
        sdcardServer.appendFile("/test.txt", altMsg.c_str());
        sdcardServer.appendFile("/test.txt", gpsLatMsg.c_str());
        sdcardServer.appendFile("/test.txt", gpsLonMsg.c_str());

        lastSentMsgTime = timeNow;
    }

}
