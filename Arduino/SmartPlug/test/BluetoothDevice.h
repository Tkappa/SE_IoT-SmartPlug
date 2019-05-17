#ifndef __BTDEVICE__
#define __BTDEVICE__

#include "Arduino.h"
#include "SoftwareSerial.h"

class BluetoothDevice{

public:

  virtual bool powerOn();
  virtual bool setup();
  virtual bool powerOff();

  virtual void resetFSM();

  virtual bool isPaired();
  virtual bool handleMessages();



  void begin(long baud){
    btChannel->begin(baud);
    baudRate=baud;
  }

  void write(uint8_t input){
    btChannel->write(input);
  }

  int read(){
    return btChannel->read();
  }
  int available(){
    return btChannel->available();
  }



protected:
  SoftwareSerial * btChannel;
  int rxPin;
  int txPin;
  long baudRate;

};

#endif
