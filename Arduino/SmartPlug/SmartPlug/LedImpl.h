#ifndef __LEDIMP__
#define __LEDIMP__

#include "Led.h"

class LedImpl: public Led{
public:
  LedImpl(int inpin){
    pin=inpin;
  }
  void begin(){
    pinMode(pin,OUTPUT);
    digitalWrite(pin,LOW);
    status=false;
  }

  void turnOn(){
    status = true;
    digitalWrite(pin,HIGH);
  }
  void turnOff(){
    status = false;
    digitalWrite(pin,LOW);
  }

  void toggle(){
    status = !status;
    digitalWrite(pin,status);
  }
  bool getStatus(){
    return status;
  }


private:
  int pin;
};

#endif
