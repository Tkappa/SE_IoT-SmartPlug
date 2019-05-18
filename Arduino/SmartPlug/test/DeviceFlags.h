#ifndef __FLAGS__
#define __FLAGS__

#include "Arduino.h"


enum LEDMODE{off=0,flashing=1,on=2};

class Flags{
public:

  static Flags * getInstance();

  bool getBTBtnRequest();
  bool getBTReady();
  bool getBTOn();
  LEDMODE getBTLedCommand();

  void setBTBtnRequest(bool in);
  void setBTReady(bool in);
  void setBTOn(bool in);
  void setBTLedCommand(LEDMODE in);


  void setWFhasSettings(bool in);
  bool getWFhasSettings();

  void setValueRead(float in);
  float getValueRead();

  // C++ 11
  // =======
  // Don't forget to declare these two. You want to make sure they
  // are unacceptable otherwise you may accidentally get copies of
  // your singleton appearing.
  Flags(Flags const&)           = delete;
  void operator=(Flags const&)  = delete;

private:
  Flags();
  static Flags * instance;

  bool BTBtnRequest;
  bool BTReady;
  bool BTOn;
  LEDMODE BTLedCommand;

  float valueRead;

  bool WFhasSettings;
};

#endif
