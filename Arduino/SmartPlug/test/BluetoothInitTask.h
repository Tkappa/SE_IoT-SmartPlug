#ifndef __BTINIT__
#define __BTINIT__


#include "Arduino.h"
#include "BluetoothTask.h"
#include "DeviceFlags.h"
#include "WordFinder.h"

enum BtInitStates{BTI_checkBtn,BTI_powerOn,BTI_enterAtMode,BTI_waitForPairable,BTI_setReady,BTI_waitBtFinish,BTI_turnOff};

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
