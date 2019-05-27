#ifndef __CTSENSOR__
#define __CTSENSOR__

#include "EmonLib.h"
#include "DeviceFlags.h"
#include "WattageReader.h"

#define CONSTCALIBRATION 60
#define CONSTVOLTAGE 230.0
#define WATTAGEWEIGTH 10
#define CTWEIGTHTICKS 5

class CTSensor:public WattageReader{
public:
  CTSensor(int AnalogPin);

  void begin();
  void read();
  bool weigth();

  private:
  void calculateWeigth();
  EnergyMonitor emon;

  double currWeigth;
  double currentValueAmpere;
  double currentValueWattage;
  int ticks;
  int pin;
};

#endif
