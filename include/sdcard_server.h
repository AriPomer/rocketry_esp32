#ifndef SDCARD_SERVER_H
#define SDCARD_SERVER_H

#include <FS.h>
#include <SD.h>
#include <SPI.h>

class SdcardServer
{
public:
  SdcardServer(int chipSelect);

  void begin();
  void listDirectory(const char *dirname, uint8_t levels);
  void createDirectory(const char *path);
  void removeDirectory(const char *path);
  void readFile(const char *path);
  void writeFile(const char *path, const char *message);
  void appendFile(const char *path, const char *message);
  void renameFile(const char *path1, const char *path2);
  void deleteFile(const char *path);
  void testFileIO(const char *path);

private:
  int chipSelectPin;
};

#endif  // SDCARD_SERVER_H
