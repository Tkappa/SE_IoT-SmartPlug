#ifndef __SETTINGS__
#define __SETTINGS__

#include "Arduino.h"

#define ROUTINELEN 144 //every 10 minutes

class Settings{
public:

  static Settings * getInstance();

  char* getServerIp();
  char* getServerPort();
  float getMaxWattage();
  char* getSecretKey();
  char* getID();
  char* getWifiPassword();
  char* getWifiSSID();

  void setWifiSSID(char* ssid);
  void setWifiPassword(char* psw);
  void setServerIp(char* ip);
  void setServerPort(char* port);
  void setRoutine(bool inroutine[ROUTINELEN]);
  void setMaxWattage(int wattage);
  void setSecretKey(char* key);
  void setID(char* inid);

  bool getRoutinePos(const unsigned int index) {
     if (index > ROUTINELEN)
        return false;
     return (bool) bitRead(routine[index/8], index%8);
  }

  void setRoutineAt(const unsigned int index, const boolean value) {
     if (index > ROUTINELEN)
        return;
     bitWrite(routine[index/8], index%8, value);
  }

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
  char serverip[30];
  char serverport[10];
  unsigned char routine[(ROUTINELEN/8)+1];
  float maxWattage;
  char secretKey[10];
  char ID[5];
  char wifiSSID[100];
  char wifiPassword[20];


};

#endif
