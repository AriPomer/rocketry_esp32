#include "web_server.h"

WebServer::WebServer()
    : server(80),
      currentTime(0),
      previousTime(0),
      timeoutTime(2000){}

void WebServer::begin()
{
    server.begin();
}

int WebServer::handleClient()
{
    WiFiClient client = server.available();
    int interval = 0;

    if (client)
    {
        interval = handleIncomingRequest(client);
    }
    
    return interval;
}

int WebServer::handleIncomingRequest(WiFiClient &client)
{
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = "";
    int interval = 0;
    
    while (client.connected() && currentTime - previousTime <= timeoutTime)
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

    interval = handleHeader();
    sendHttpResponse(client);
    resetConnection(client);
    return interval;
}
void WebServer::sendHttpResponse(WiFiClient &client) {
    // Construct the entire HTML response as a single string
    String httpResponse = "HTTP/1.1 200 OK\r\n"
                          "Content-type:text/html\r\n"
                          "Connection: close\r\n\r\n"
                          "<!DOCTYPE html><html><head><title>LED Control</title>"
                          "<style>"
                          "body { display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; font-family: Arial, sans-serif; }"
                          "h1 { font-size: 72px; }"
                          "button { font-size: 48px; padding: 40px 80px; margin: 20px; border: none; border-radius: 16px; cursor: pointer; }"
                          "button.on { background-color: #4CAF50; color: white; }"
                          "button.off { background-color: #f44336; color: white; }"
                          "input { font-size: 48px; padding: 20px; margin: 20px; text-align: center; }"
                          "</style>"
                          "</head><body>"
                          "<div style='text-align:center;'>"
                          "<h1>LED Control</h1>"
                          "<p><a href=\"/led/on\"><button class='on'>Turn On LED</button></a></p>"
                          "<p><a href=\"/led/off\"><button class='off'>Turn Off LED</button></a></p>"
                          "<form action='/led/set' method='get'>"
                          "<input type='text' id='number' name='number' placeholder='Enter a number'>"
                          "<button type='submit'>Set Interval</button>"
                          "</form>"
                          "</div>"
                          "</body></html>";

    // Send the entire response at once
    client.print(httpResponse);
}


int WebServer::handleHeader()
{
    if (header.indexOf("GET /favicon.ico") >= 0)
    {
        return 0;
    }

    Serial.print(header);

    if (header.indexOf("GET /led/on") >= 0)
    {
        Serial.println("Turning On LED");
        digitalWrite(LED_BUILTIN, HIGH);
    }

    if (header.indexOf("GET /led/off") >= 0)
    {
        Serial.println("Turning Off LED");
        digitalWrite(LED_BUILTIN, LOW);
    }

    if (header.indexOf("GET /led/set") >= 0)
    {
        int interval = 0;
        int pos = header.indexOf("number=");
        if (pos != -1)
            interval = header.substring(pos + 7).toInt();

        Serial.println("Setting interval to " + String(interval));
        return interval;
    }
    
    return 0;
}

void WebServer::resetConnection(WiFiClient &client)
{
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
}
