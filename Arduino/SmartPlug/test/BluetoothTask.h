#ifndef __BTTASK__
#define __BTTASK__


#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Task.h"

class BluetoothTask: public Task{

public:
  BluetoothTask(int tx,int rx);

  void begin(long baudRate);
  int available();
  int read();
  void write(uint8_t input);
  
  

protected:

  SoftwareSerial * btChannel;
  int rxPin;
  int txPin;
  long baudRate;
};

#endif
