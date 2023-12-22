#ifndef WIFI_SERVER_H
#define WIFI_SERVER_H

#include <Arduino.h>
#include <WiFi.h>

enum BlinkLed
{
    ENABLE,
    DISABLE
};

class WifiServer
{
public:
    WifiServer(const char* ssid, const char* password, BlinkLed state);
    void begin();
    void blinkWifi();
    void setInterval(int interval);

private:
    const char* ssid;
    const char* password;
    unsigned long previousTime;
    bool printIP;
    bool blinkLed;
    int interval;
};

#endif // WIFI_SERVER_H
