#include "mpu_server.h"
#include <Wire.h>

void MpuServer::begin()
{
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin())
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
    {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  printAccelRange();

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  printGyroRange();

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  printFilterBandwidth();

  Serial.println("");
  delay(100);
}

String MpuServer::getData()
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  String data = getAccelValues(a) + getGyroValues(g) + getTemperature(temp);
  return data;
}

void MpuServer::printAccelRange()
{
  switch (mpu.getAccelerometerRange())
  {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
}

void MpuServer::printGyroRange()
{
  switch (mpu.getGyroRange())
  {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }
}

void MpuServer::printFilterBandwidth()
{
  switch (mpu.getFilterBandwidth())
  {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
}

String MpuServer::getAccelValues(const sensors_event_t &a)
{
  return "Acceleration X: " + String(a.acceleration.x) +
         ", Y: " + String(a.acceleration.y) +
         ", Z: " + String(a.acceleration.z) + " m/s^2\n";
}

String MpuServer::getGyroValues(const sensors_event_t &g)
{
  return "Rotation X: " + String(g.gyro.x) +
         ", Y: " + String(g.gyro.y) +
         ", Z: " + String(g.gyro.z) + " rad/s\n";
}

String MpuServer::getTemperature(const sensors_event_t &temp)
{
  return "Temperature: " + String(temp.temperature) + " degC\n";
}
