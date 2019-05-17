#ifndef __WIFIESP8266__
#define __WIFIESP8266__

#include "WifiDevice.h"
#include "DeviceSettings.h"
#include "DeviceFlags.h"
#include "SoftwareSerial.h"
#include "WordFinder.h"
#include "CommandParser.h"

enum ESPsetup {ESP_getSettings,ESP_checkconn,ESP_disconnect,ESP_connect,ESP_skipTick,ESP_readResponse};

class WifiESP8266 : public WifiDevice{

public:
  WifiESP8266(int tx,int rx);

  int setup();
  bool checkConnection();
  bool disconnect();
  bool connect();

  bool postData();
  bool getCommands();
  bool pingBack();

private:
  ESPsetup setupStatus;

  char serverip[30];
  char serverport[4];
  char secretKey[5];
  char ID[2];
  char wifiSSID[20];
  char wifiPassword[20];


  WordFinder * currToFind;
  CommandParser * parser;
  int setupBusyWaiting;
};

#endif
