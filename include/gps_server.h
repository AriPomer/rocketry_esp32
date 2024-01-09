#ifndef GPS_SERVER_H
#define GPS_SERVER_H

#include <Adafruit_GPS.h>

// TX-RX harware serial number on esp32
#define GPSSerial Serial2

class GPSServer {
public:
  GPSServer();
  void begin();
  void checkData();
  String getTime();
  String getDate();
  String getFix();
  String getQuality();
  String getLocation();
  String getSpeed();
  String getAngle();
  String getAltitude();
  String getSatellites();
  String getAntennaStatus();
public:
  Adafruit_GPS GPS;
};

#endif // GPS_SERVER_H
