#ifndef __BTDEVICE__
#define __BTDEVICE__

#include "Arduino.h"
#include "SoftwareSerial.h"

class BluetoothDevice{

public:

  /*BluetoothDevice(int tx,int rx){
      txPin=tx;
      rxPin=rx;
      btChannel= new SoftwareSerial(tx, rx);
  }*/

  virtual bool powerOn();
  virtual bool setup();
  virtual bool powerOff();

  virtual void resetFSM();

  virtual bool isPaired();
  virtual bool handleMessages();



  //virtual void begin(long baud){
  //  btChannel->begin(baud);
  //  baudRate=baud;
  //}a

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
