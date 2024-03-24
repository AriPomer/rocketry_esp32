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
    return String(pressure);
}

String MPLSserver::getAltitude()
{
    float altitude = baro.getAltitude();
    return String(altitude);
}

String MPLSserver::getTemperature()
{
    float temperature = baro.getTemperature();
    return String(temperature);
}
