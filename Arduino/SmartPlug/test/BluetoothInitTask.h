#ifndef __BTINIT__
#define __BTINIT__

#define MAXTRIES 30

#include "Arduino.h"
#include "DeviceFlags.h"
#include "BluetoothDevice.h"
#include "Task.h"

enum BtInitStates{BTI_checkBtn,BTI_powerOn,BTI_waitReady,BTI_setReady,BTI_waitBtFinish,BTI_turnOff};

class BluetoothInit: public Task{


public:
  BluetoothInit(BluetoothDevice * inCurrDevice);

  void init(int basePeriod);
  void tick();


private:
  BluetoothDevice * currDevice;
  BtInitStates state;

};

#endif
