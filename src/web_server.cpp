#include "web_server.h"

WebServer::WebServer(MpuServer &mpuServer)
    : server(80),
      mpuServer(mpuServer),
      currentTime(0),
      previousTime(0),
      timeoutTime(2000){}

void WebServer::begin()
{
    server.begin();
}

void WebServer::handleClient()
{
    WiFiClient client = server.available();

    if (client)
    {
        handleIncomingRequest(client);
    }
}

void WebServer::handleIncomingRequest(WiFiClient &client)
{
    Serial.println("New Client.");
    String currentLine = "";
    int interval = 0;
    
    while (client.connected())
    {
        currentTime = millis();

        if (client.available())
        {
            char c = client.read();
            header += c;

            if (c == '\n')
            {
                if (currentLine.length() == 0)
                    break;
                else
                    currentLine = "";
            }
            else if (c != '\r')
            {
                currentLine += c;
            }
        }
    }

    handleHeader();
    sendHttpResponse(client);
    resetConnection(client);
}


void WebServer::sendHttpResponse(WiFiClient &client) {
    // Get MPU data
    String mpuInfo = mpuServer.getData();
    Serial.println(mpuInfo);
    mpuInfo.replace("\n", "<br>");
    Serial.println(mpuInfo);

    // Construct the entire HTML response as a single string
    String httpResponse = "HTTP/1.1 200 OK\r\n"
                          "Content-type:text/html\r\n"
                          "Connection: close\r\n\r\n"
                          "<!DOCTYPE html><html><head><title>MPU Info</title>"
                          "<style>"
                          "body { display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; font-family: Arial, sans-serif; }"
                          "h1 { font-size: 72px; }"
                          "p { font-size: 24px; }"  // Adjust the font size as needed
                          "</style>"
                          "</head><body>"
                          "<div style='text-align:center;'>"
                          "<h1>MPU Information</h1>"
                          "<p>" + mpuInfo + "</p>"
                          "</div>"
                          "</body></html>";

    // Send the entire response at once
    client.print(httpResponse);
}



void WebServer::handleHeader()
{
    if (header.indexOf("GET /favicon.ico") >= 0)
    {
        return;
    }

    Serial.print(header);

    if (header.indexOf("GET /mpu/info") >= 0)
    {
        Serial.println("Turning On LED");
        digitalWrite(LED_BUILTIN, HIGH);
    }

    if (header.indexOf("GET /sdcard/info") >= 0)
    {
        Serial.println("Turning Off LED");
        digitalWrite(LED_BUILTIN, LOW);
    }
}

void WebServer::resetConnection(WiFiClient &client)
{
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
}
