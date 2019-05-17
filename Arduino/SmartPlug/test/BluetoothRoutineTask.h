#ifndef __BTROUT__
#define __BTROUT__

#define MAXTRIES 30

#include "Arduino.h"
#include "DeviceFlags.h"
#include "CommandParser.h"
#include "BluetoothDevice.h"
#include "Task.h"
#include "WordFinder.h"

enum BtRoutineStates{BTR_waitReady,BTR_waitPair,BTR_handleMessages,BTR_turnOff};

class BluetoothRoutine: public Task{


public:
  BluetoothRoutine(BluetoothDevice * inCurrDevice);


  void init(int basePeriod);
  void tick();

protected:

  BluetoothDevice * currDevice;
  BtRoutineStates state;
  int ticksAlone;
};

#endif
