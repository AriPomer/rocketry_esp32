#include "mpl_server.h"

MPLSserver::MPLSserver() : baro() {}

void MPLSserver::begin(float SLP)
{
    while (!baro.begin());

    baro.setSeaPressure(SLP);
}

String MPLSserver::getPressure()
{
    float pressure = baro.getPressure();
    return "pressure = " + String(pressure) + " hPa";
}

String MPLSserver::getAltitude()
{
    float altitude = baro.getAltitude();
    return "altitude = " + String(altitude) + " m";
}

String MPLSserver::getTemperature()
{
    float temperature = baro.getTemperature();
    return "temperature = " + String(temperature) + " C";
}
