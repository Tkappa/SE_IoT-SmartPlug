#ifndef __WIFITASK__
#define __WIFITASK__


#include "Arduino.h"
#include "WifiDevice.h"
#include "SoftwareSerial.h"
#include "Task.h"

class WifiTask: public Task{

public:
  WifiTask(int tx,int rx,WifiDevice* inputDevice);

  void begin(long baudRate);
  int available();
  int read();
  void write(uint8_t input);



protected:

  WifiDevice * currentDevice;
  SoftwareSerial * wifiChannel;
  int rxPin;
  int txPin;
  long baudRate;
};

#endif
