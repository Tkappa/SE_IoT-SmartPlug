#ifndef __WFROUT__
#define __WFROUT__

#define MAXTRIES 30

#include "Arduino.h"
#include "WifiTask.h"
#include "DeviceFlags.h"
#include "CommandParser.h"
#include "WordFinder.h"

enum WIFI_RoutineStates{WFR_hasSettings,WFR_setup,WFR_postData,WFR_getCommand,WFR_handleCommand,WFR_pingServer};

class WifiRoutine: public WifiTask{


public:
  WifiRoutine(int tx,int rx,WifiDevice * inputDevice,long baud);


  void init(int basePeriod);
  void tick();

protected:

  WIFI_RoutineStates state;

private:

  //WordFinder * currToFind;
  //CommandParser * parser;
};

#endif
