#ifndef __POWRELAY__
#define __POWRELAY__

#include "Arduino.h"

class PowerRelay{
public:
  virtual void begin(){
    pinMode(pin,OUTPUT);
  }
  virtual void powerOn();
  virtual void powerOff();
protected:
  int pin;
};

#endif
