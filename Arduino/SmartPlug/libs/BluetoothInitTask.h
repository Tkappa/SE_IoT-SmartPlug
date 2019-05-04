#ifndef __BTINIT__
#define __BTINIT__


#include "Arduino.h"
#include "BluetoothTask.h"
#include "DeviceFlags.h"
#include "WordFinder.h"

enum BtInitStates{checkBtn,powerOn,enterAtMode,waitForPairable,setReady};

class BluetoothInit: public BluetoothTask{


public:
  BluetoothInit(int tx,int rx,int vcc,int at,long baud);


  void init(int basePeriod);
  void tick();

protected:

  BtInitStates state;
  int vccPin;
  int atPin;

private:
  WordFinder * currToFind;
};

#endif
