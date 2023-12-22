#ifndef MPU_SERVER_H
#define MPU_SERVER_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

class MpuServer
{
public:
  void begin();
  String getData();

private:
  Adafruit_MPU6050 mpu;

  void printAccelRange();
  void printGyroRange();
  void printFilterBandwidth();
  String getAccelValues(const sensors_event_t& a);
  String getGyroValues(const sensors_event_t& g);
  String getTemperature(const sensors_event_t& temp);
};

#endif  // MPU_SERVER_H
