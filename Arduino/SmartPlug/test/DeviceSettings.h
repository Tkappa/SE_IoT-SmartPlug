#ifndef __SETTINGS__
#define __SETTINGS__

#include "Arduino.h"

#define ROUTINELEN 288 //every 5 minutes

class Settings{
public:

  static Settings * getInstance();

  String getServerIp();
  String getServerPort();
  bool getRoutinePos(int pos);
  float getMaxWattage();
  int getSecretKey();
  int getID();

  void setServerIp(String ip);
  void setServerPort(String port);
  void setRoutine(bool inroutine[ROUTINELEN]);
  void setMaxWattage(int wattage);
  void setSecretKey(int key);
  void setID(int inid);


  // C++ 11
  // =======
  // Don't forget to declare these two. You want to make sure they
  // are unacceptable otherwise you may accidentally get copies of
  // your singleton appearing.
  Settings(Settings const&)        = delete;
  void operator=(Settings const&)  = delete;

private:
  Settings();
  static Settings * instance;
  String serverip;
  String serverport;
  bool routine[ROUTINELEN];
  float maxWattage;
  int secretKey;
  int ID;


};

#endif
