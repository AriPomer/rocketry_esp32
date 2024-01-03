#include "gps_server.h"

GPSServer::GPSServer(HardwareSerial& serialPort) : GPS(&serialPort), serialPort(serialPort){}

void GPSServer::begin()
{
    Serial.begin(115200);
    GPS.begin(9600);
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    GPS.sendCommand(PGCMD_ANTENNA);

    delay(1000);

    serialPort.println(PMTK_Q_RELEASE);
}

String GPSServer::getTime()
{
    String Hour = (GPS.hour < 10) ? "0" + String(GPS.hour) : String(GPS.hour);
    String Minute = (GPS.minute < 10) ? "0" + String(GPS.minute) : String(GPS.minute);
    String Second = (GPS.seconds < 10) ? "0" + String(GPS.seconds) : String(GPS.seconds);

    return (Hour + ":" + Minute + ":" + Second);
}

String GPSServer::getDate()
{
    return (String(GPS.day) + "/" + String(GPS.month) + "/20" + String(GPS.year));
}

String GPSServer::getFix()
{
    return (GPS.fix) ? "1" : "0";
}

String GPSServer::getQuality()
{
    return String(GPS.fixquality);
}

String GPSServer::getLocation()
{
    return (String(GPS.latitude, 4) + "N, " + String(GPS.longitude, 4) +"W");
}

String GPSServer::getSpeed()
{
    return String(GPS.speed);
}

String GPSServer::getAngle()
{
    return String(GPS.angle);
}

String GPSServer::getAltitude()
{
    return String(GPS.altitude);
}

String GPSServer::getSatellites()
{
    return String(GPS.satellites);
}

String GPSServer::getAntennaStatus()
{
    return String(GPS.antenna);
}