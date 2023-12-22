#include "wifi_server.h"

WifiServer::WifiServer(const char* ssid, const char* password, BlinkLed state)
    : ssid(ssid),
      password(password),
      previousTime(0),
      printIP(false),
      blinkLed(state),
      interval(2000)
      {}

void WifiServer::begin()
{
    pinMode(LED_BUILTIN, OUTPUT);
    WiFi.begin(ssid, password);
    Serial.println("Starting WiFi");
}

void WifiServer::setInterval(int interval)
{
    this->interval = interval;
}

void WifiServer::blinkWifi()
{
    if(WiFi.status() == WL_CONNECTED)
    {   
        unsigned long currentInterval= millis() % interval;

        if(currentInterval == 0 && !printIP)
        {
            Serial.println("IP address: " + WiFi.localIP().toString());
            printIP = true;
        }
        
        if(currentInterval != 0)
            printIP = false;

        if (blinkLed == BlinkLed::ENABLE && currentInterval >= 0 && currentInterval <= (interval / 4))
            digitalWrite(LED_BUILTIN, HIGH);
        else
            digitalWrite(LED_BUILTIN, LOW);
    }

    if(WiFi.status() != WL_CONNECTED)
    {
        unsigned long currentInterval = millis() % 1000;

        if (currentInterval >= 0 && currentInterval <= 500)
            digitalWrite(LED_BUILTIN, HIGH);
        else
            digitalWrite(LED_BUILTIN, LOW);
    }
}
