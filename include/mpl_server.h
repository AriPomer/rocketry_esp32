#ifndef MPL_SERVER_h
#define MPL_SERVER_h

#include <Arduino.h>
#include <Adafruit_MPL3115A2.h>

class MPLSserver
{
public:
    MPLSserver();
    void begin(float SLP);
    String getPressure();
    String getAltitude();
    String getTemperature();

private:
    Adafruit_MPL3115A2 baro;
};

#endif
