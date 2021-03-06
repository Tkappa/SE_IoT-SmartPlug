#ifndef __WIFIDEVICE__
#define __WIFIDEVICE__

#include "Arduino.h"
#include "SoftwareSerial.h"

class WifiDevice{
public:

//It's int because it needs to return an error status
virtual int setup();

virtual int postData();
virtual bool getCommands();
virtual bool pingBack();
virtual void resetFSM();

void begin(long baud){
  wifiChannel->begin(baud);
  baudRate=baud;
}

void write(uint8_t input){
  wifiChannel->write(input);
}

int read(){
  return wifiChannel->read();
}

int available(){
  return wifiChannel->available();
}

protected:
  SoftwareSerial * wifiChannel;
  int rxPin;
  int txPin;
  long baudRate;
};

#endif
