#ifndef GPS_SERVER_H
#define GPS_SERVER_H

#include <Adafruit_GPS.h>

class GPSServer {
public:
  GPSServer(HardwareSerial& serialPort);
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
  HardwareSerial& serialPort;
};

#endif // GPS_SERVER_H
