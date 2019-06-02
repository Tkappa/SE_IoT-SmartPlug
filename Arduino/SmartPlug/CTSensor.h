#ifndef __CTSENSOR__
#define __CTSENSOR__

#include "EmonLib.h"
#include "DeviceFlags.h"
#include "WattageReader.h"

#define CONSTCALIBRATION 60
#define CONSTVOLTAGE 230.0
#define WATTAGEWEIGHT 10
#define CTWEIGHTTICKS 5

//Debug readings
//#define CTDEBUG

class CTSensor:public WattageReader{
public:
  CTSensor(int AnalogPin);

  void begin();
  void read();
  bool weight();

  private:
  void calculateweight();
  EnergyMonitor emon;

  double currweight;
  double currentValueAmpere;
  double currentValueWattage;
  int ticks;
  int pin;
};

#endif
