#ifndef __KYRELAY__
#define __KYRELAY__

#include "PowerRelay.h"

class KYPowRelay: public PowerRelay{
public:
  KYPowRelay(int inpin){
    pin=inpin;
  }
  void begin(){
    PowerRelay::begin();
  }
  void powerOn(){
    digitalWrite(pin,HIGH);
  }
  void powerOff(){
    digitalWrite(pin,LOW);
  }
};

#endif
