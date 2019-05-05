#ifndef __BTROUT__
#define __BTROUT__

#define MAXTRIES 30

#include "Arduino.h"
#include "BluetoothTask.h"
#include "DeviceFlags.h"
#include "CommandParser.h"
#include "WordFinder.h"

enum BtRoutineStates{BTR_waitReady,BTR_waitPair,BTR_handleMessages,BTR_turnOff};

class BluetoothRoutine: public BluetoothTask{


public:
  BluetoothRoutine(int tx,int rx,int state,long baud);


  void init(int basePeriod);
  void tick();

protected:

  BtRoutineStates state;
  int statePin;
  int ticksAlone;
  
private:
  WordFinder * currToFind;
  CommandParser * parser;
};

#endif
