#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <WiFi.h>

class WebServer
{
public:
    WebServer();
    void begin();
    int handleClient();

private:
    WiFiServer server;
    String header;
    unsigned long currentTime;
    unsigned long previousTime;
    int timeoutTime;

    int handleIncomingRequest(WiFiClient &client);
    void sendHttpResponse(WiFiClient &client);
    int handleHeader();
    void resetConnection(WiFiClient &client);
};

#endif // WEB_SERVER_H
