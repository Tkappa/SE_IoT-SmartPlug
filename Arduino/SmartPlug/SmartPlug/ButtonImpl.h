#ifndef __BTNIMPL__
#define __BTNIMPL__

#include "Button.h"

class BtnImpl:public Button{
  public:
  BtnImpl(int inpin){
    pin=inpin;
  }
  void begin(){
    pinMode(pin,INPUT);
  }
  bool isPressed(){
    bool value = digitalRead(pin);
    return value;
  }

private:
  int pin;
};

#endif
