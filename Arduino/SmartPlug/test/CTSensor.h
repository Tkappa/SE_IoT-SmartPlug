#ifndef __CTSENSOR__
#define __CTSENSOR__

#include "EmonLib.h"
#include "DeviceFlags.h"
#include "WattageReader.h"

#define CONSTCALIBRATION 111.1
#define CONSTVOLTAGE 230.0

class CTSensor:public WattageReader{
public:
  CTSensor(int AnalogPin);

  void begin();
  void read();
  bool weigth();

  private:
  EnergyMonitor emon;
  //Used in init to move the value to 0
  double currWeigth;
  double currentValueAmpere;
  double currentValueWattage;
  int ticks;
  int pin;
};

#endif
