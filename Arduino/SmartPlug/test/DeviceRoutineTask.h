#ifndef __DEVINIT__
#define __DEVINIT__

#define MAXTRIES 30
#define ROUTINELEN 144

#include "Arduino.h"
#include "DeviceFlags.h"
#include "DeviceSettings.h"
#include <EEPROM.h>
#include "WattageReader.h"
#include "Task.h"

enum InitStatus{INT_getSettings,INT_adjustReadings,INT_setupDone};



class DeviceRout: public Task{


public:
  DeviceRout(WattageReader * inCurrDevice);

  void init(int basePeriod);
  void tick();


private:
  WattageReader * currDevice;
  InitStatus state;

};

#endif
