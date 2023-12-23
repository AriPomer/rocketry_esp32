#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include "mpu_server.h"

class WebServer
{
public:
    WebServer(MpuServer &mpuServer);
    void begin();
    void handleClient();

private:
    WiFiServer server;
    MpuServer &mpuServer;
    String header;
    unsigned long currentTime;
    unsigned long previousTime;
    int timeoutTime;

    void handleIncomingRequest(WiFiClient &client);
    void sendHttpResponse(WiFiClient &client);
    void handleHeader();
    void resetConnection(WiFiClient &client);
};

#endif // WEB_SERVER_H
