#ifndef __WFROUT__
#define __WFROUT__

#define MAXTRIES 30

#include "Arduino.h"
#include "Task.h"
#include "WifiDevice.h"
#include "DeviceFlags.h"

enum WIFI_RoutineStates{WFR_hasSettings,WFR_setup,WFR_postData,WFR_getCommand,WFR_handleCommand,WFR_pingServer};

class WifiRoutine: public Task{


public:
  WifiRoutine(WifiDevice * inputDevice);

  void init(int basePeriod);
  void tick();
  void reset();

protected:
  WifiDevice * currentDevice;
  WIFI_RoutineStates state;
  int errorTollerance;
};

#endif
